#pragma once

#include "config.hpp"
#include "buzzer.hpp"
#include <cstdint>

namespace tone
{
    constexpr uint16_t C4 = 262;
    constexpr uint16_t D4 = 294;
    constexpr uint16_t E4 = 330;
    constexpr uint16_t F4 = 349;
    constexpr uint16_t G4 = 392;
    constexpr uint16_t A4 = 440;
    constexpr uint16_t B4 = 494;
    constexpr uint16_t C5 = 523;
    constexpr uint16_t rest = 0;
}

namespace duration
{
    constexpr uint16_t half = 800;
    constexpr uint16_t quarter = 400;
    constexpr uint16_t eighth = 200;
}

struct DurationConfig
{
    uint16_t durationMs;
    uint8_t ledCount;
};

constexpr DurationConfig durationConfigs[] = {
    {duration::half, 4},
    {duration::quarter, 2},
    {duration::eighth, 1},
};

constexpr int durationCount = sizeof(durationConfigs) / sizeof(durationConfigs[0]);

struct Note
{
    uint16_t tone;
    uint16_t duration;
};

class Melody
{
private:
    Note mNotes[melody::maxNotes];
    uint8_t mNoteCount = 0;
    uint32_t mLastNotePlayedMs = 0;
    uint8_t mCurrentNoteIndex = 0;
    bool mIsPlaying = false;

public:
    void addNote(uint16_t tone, uint16_t duration)
    {
        if (mNoteCount < melody::maxNotes)
        {
            mNotes[mNoteCount++] = {tone, duration};
        }
    }

    bool playMelody(uint32_t currentTimeMs)
    {
        if (mCurrentNoteIndex >= mNoteCount)
        {
            stopNote();
            mIsPlaying = false;
            return false;
        }

        uint32_t elapsedTime = currentTimeMs - mLastNotePlayedMs;

        uint16_t frequency = mNotes[mCurrentNoteIndex].tone;
        uint16_t durationMs = mNotes[mCurrentNoteIndex].duration;

        if (mLastNotePlayedMs == 0)
        {
            mLastNotePlayedMs = currentTimeMs;
            elapsedTime = 0;
        }

        if (elapsedTime >= durationMs)
        {
            mCurrentNoteIndex++;
            if (mCurrentNoteIndex >= mNoteCount)
            {
                stopNote();
                mIsPlaying = false;
                return false;
            }

            mLastNotePlayedMs = currentTimeMs;
            elapsedTime = currentTimeMs - mLastNotePlayedMs;
            frequency = mNotes[mCurrentNoteIndex].tone;
            durationMs = mNotes[mCurrentNoteIndex].duration;
        }

        const bool isPauseBetweenNotes = elapsedTime > (durationMs - melody::restBetweenNotesMs);
        if (isPauseBetweenNotes)
        {
            stopNote();
            mIsPlaying = false;
            return true;
        }

        if (mIsPlaying)
        {
            return true;
        }

        if (frequency == tone::rest)
        {
            stopNote();
        }
        else
        {
            playNote(frequency);
            
        }
        mIsPlaying = true;

        return true;
    }

    void stopMelody()
    {
        stopNote();
        mIsPlaying = false;
        mCurrentNoteIndex = 0;
        mLastNotePlayedMs = 0;
    }

    void clearMelody()
    {
        stopMelody();
        mNoteCount = 0;
        mCurrentNoteIndex = 0;
        mLastNotePlayedMs = 0;
    }
};