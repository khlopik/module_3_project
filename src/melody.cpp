#include "melody.hpp"
#include "buzzer.hpp"

void Melody::addNote(uint16_t tone, uint16_t duration)
{
    if (mNoteCount < melodyConfig::maxNotes)
    {
        mNotes[mNoteCount++] = {tone, duration};
    }
}

bool Melody::playMelody(uint32_t currentTimeMs)
{
    if (mCurrentNoteIndex >= mNoteCount)
    {
        buzzer::stopNote();
        mIsPlaying = false;
        return false;
    }

    uint32_t elapsedTime = currentTimeMs - mLastNotePlayedMs;

    uint16_t frequency = mNotes[mCurrentNoteIndex].tone;
    uint16_t durationMs = mNotes[mCurrentNoteIndex].duration;

    // The first note played
    if (mLastNotePlayedMs == 0)
    {
        mLastNotePlayedMs = currentTimeMs;
        elapsedTime = 0;
    }

    // Check if the current note has finished playing
    if (elapsedTime >= durationMs)
    {
        mCurrentNoteIndex++;
        if (mCurrentNoteIndex >= mNoteCount)
        {
            buzzer::stopNote();
            mIsPlaying = false;
            return false;
        }

        mLastNotePlayedMs = currentTimeMs;
        elapsedTime = currentTimeMs - mLastNotePlayedMs;
        frequency = mNotes[mCurrentNoteIndex].tone;
        durationMs = mNotes[mCurrentNoteIndex].duration;
    }

    const bool isPauseBetweenNotes = elapsedTime > (durationMs - melodyConfig::restBetweenNotesMs);
    if (isPauseBetweenNotes)
    {
        buzzer::stopNote();
        mIsPlaying = false;
        return true;
    }

    if (mIsPlaying)
    {
        return true;
    }

    if (frequency == tone::rest)
    {
        buzzer::stopNote();
    }
    else
    {
        buzzer::playNote(frequency);
    }
    mIsPlaying = true;

    return true;
}

void Melody::stopMelody()
{
    buzzer::stopNote();
    mIsPlaying = false;
    mCurrentNoteIndex = 0;
    mLastNotePlayedMs = 0;
}

void Melody::clearMelody()
{
    stopMelody();
    mNoteCount = 0;
    mCurrentNoteIndex = 0;
    mLastNotePlayedMs = 0;
}