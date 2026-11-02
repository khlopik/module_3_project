#include "config.hpp"
#include "gpio.hpp"
#include "servo.hpp"
#include "buzzer.hpp"
#include "encoder.hpp"
#include "melody.hpp"
#include "duration_leds.hpp"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

constexpr uint16_t tones[] = {tone::C4, tone::D4, tone::E4, tone::F4, tone::G4, tone::A4, tone::B4, tone::C5, tone::rest};
constexpr int toneCount = sizeof(tones) / sizeof(tones[0]);

// constexpr uint16_t durations[] = {duration::half, duration::quarter, duration::eighth};
// constexpr int durationCount = sizeof(durations) / sizeof(durations[0]);

int currentNoteIndex = -1;
uint16_t currentDurationIndex = 0;
uint32_t lastNotePlayedMs = 0;

inline uint16_t getAngleForNoteIndex(uint16_t noteIndex)
{
    return noteIndex * servo::maxAngle / (toneCount - 1);
}

enum State
{
    SelectNote,
    PreviewPlayNote,
    SaveNoteToMelody,
    PlayMelody,
    StopMelody,
};

State currentState = SelectNote;
Melody currentMelody;
DurationLeds durationLeds;

void handleButtonPress()
{
    if (consumePlayMelodyRequest())
    {
        if (currentState == PlayMelody)
        {
            currentState = StopMelody;
        }
        else
        {
            currentState = PlayMelody;
        }
        return;
    }

    if (consumeResetMelodyRequest())
    {
        currentMelody.clearMelody();
        currentState = SelectNote;
        currentNoteIndex = -1;
        currentDurationIndex = 0;
        durationLeds.showDuration(currentDurationIndex);
        return;
    }

    if (consumeChangeDurationRequest())
    {
        if (currentState != SelectNote && currentState != PreviewPlayNote)
        {
            return;
        }
        currentDurationIndex = (currentDurationIndex + 1) % durationCount;
        durationLeds.showDuration(currentDurationIndex);
        ESP_LOGI("DURATION", "New duration index: %d", currentDurationIndex);
    }

    if (consumeAddNoteRequest())
    {
        if (currentState != SelectNote && currentState != PreviewPlayNote)
        {
            return;
        }
        currentState = SaveNoteToMelody;
    }
}

void handleNoteChange(uint32_t currentTimeMs)
{
    int count = getEncoderCount();

    const int newNoteIndex = ((count % toneCount) + toneCount) % toneCount;

    if (newNoteIndex != currentNoteIndex)
    {
        ESP_LOGI("ENCODER", "newNoteIndex: %d", newNoteIndex);
        currentNoteIndex = newNoteIndex;
        uint16_t frequency = tones[currentNoteIndex];
        uint16_t angle = getAngleForNoteIndex(currentNoteIndex);
        ESP_LOGI("SERVO", "angle: %d", angle);
        servoSetAngle(angle);
        if (frequency == tone::rest)
        {
            stopNote();
        }
        else
        {
            playNote(frequency);
        }

        lastNotePlayedMs = currentTimeMs;
        currentState = PreviewPlayNote;
    }
}

void setup()
{
    gpioInit();
    encoderInit();
    servoInit();
    buzzerInit();
    servoSetAngle(90);
    durationLeds.showDuration(currentDurationIndex);
}

void loop()
{
    while (true)
    {
        handleButtonPress();
        const uint32_t currentTimeMs = xTaskGetTickCount() * portTICK_PERIOD_MS;
        switch (currentState)
        {
        case SelectNote:
        {
            handleNoteChange(currentTimeMs);
        }
        break;
        case PreviewPlayNote:
        {
            handleNoteChange(currentTimeMs);
            if (currentTimeMs - lastNotePlayedMs >= durationConfigs[currentDurationIndex].durationMs)
            {
                stopNote();
                currentState = SelectNote;
            }
        }
        break;
        case SaveNoteToMelody:
        {
            stopNote();
            currentMelody.addNote(tones[currentNoteIndex], durationConfigs[currentDurationIndex].durationMs);
            currentNoteIndex = -1;
            currentDurationIndex = 0;
            durationLeds.showDuration(currentDurationIndex);
            currentState = SelectNote;
            encoderReset();
        }
        break;
        case PlayMelody:
        {
            // Handle play melody logic
            const bool isPlaying = currentMelody.playMelody(currentTimeMs);
            if (!isPlaying)
            {
                currentState = StopMelody;
            }
        }
        break;
        case StopMelody:
        {
            currentMelody.stopMelody();
        }
        break;
        }

        vTaskDelay(pdMS_TO_TICKS(superloop::yieldDelayMs));

        // vTaskDelay(pdMS_TO_TICKS(100));
        // servoSetAngle(0);
        // playNote(440, 500);
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // servoSetAngle(180);
        // stopNote();
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // servoSetAngle(90);
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // vTaskDelay(pdMS_TO_TICKS(superloop::yieldDelayMs));
    }
}

extern "C" void app_main()
{
    setup();
    loop();
}
