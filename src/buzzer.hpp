#ifndef BUZZER_HPP
#define BUZZER_HPP

#include <cstdint>

void buzzerInit();
void playNote(uint16_t frequency, uint16_t durationMs);
void stopNote();

#endif // BUZZER_HPP