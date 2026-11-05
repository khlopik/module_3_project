#pragma once

#include "config.hpp"
#include <cstdint>

struct Note
{
    uint16_t tone;
    uint16_t duration;
};

class Melody
{
private:
    Note notes_[melodyConfig::maxNotes];
    uint8_t noteCount_ = 0;
    uint32_t lastNotePlayedMs_ = 0;
    uint8_t currentNoteIndex_ = 0;
    bool isPlaying_ = false;
    uint8_t getNotesLeftToPlay() const;

public:
    uint8_t getNoteCount() const;
    void addNote(uint16_t tone, uint16_t duration);
    uint8_t playMelody(uint32_t currentTimeMs);
    void stopMelody();
    void clearMelody();
};