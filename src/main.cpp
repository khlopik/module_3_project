#include "config.hpp"
#include "gpio.hpp"
#include "servo.hpp"
#include "buzzer.hpp"
#include "encoder.hpp"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

namespace note
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

constexpr uint16_t notes[] = {note::C4, note::D4, note::E4, note::F4, note::G4, note::A4, note::B4, note::C5, note::rest};
constexpr int noteCount = sizeof(notes) / sizeof(notes[0]);

constexpr uint16_t durations[] = {duration::half, duration::quarter, duration::eighth};
constexpr int durationCount = sizeof(durations) / sizeof(durations[0]);

int currentNoteIndex = -1;
uint16_t currentDurationIndex = 0;
uint32_t lastNotePlayedMs = 0;

inline uint16_t getAngleForNoteIndex(uint16_t noteIndex)
{
    return noteIndex * servo::maxAngle / (noteCount - 1);
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

void handleNoteChange(uint32_t currentTimeMs)
{
    int count = getEncoderCount();

    const int newNoteIndex = ((count % noteCount) + noteCount) % noteCount;

    if (newNoteIndex != currentNoteIndex)
    {
        ESP_LOGI("ENCODER", "newNoteIndex: %d", newNoteIndex);
        currentNoteIndex = newNoteIndex;
        uint16_t frequency = notes[currentNoteIndex];
        uint16_t angle = getAngleForNoteIndex(currentNoteIndex);
        ESP_LOGI("SERVO", "angle: %d", angle);
        servoSetAngle(angle);
        if (frequency == note::rest)
        {
            stopNote();
        }
        else
        {
            playNote(frequency, currentDurationIndex);
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
}

void loop()
{
    while (true)
    {
        const uint32_t currentTimeMs = xTaskGetTickCount() * portTICK_PERIOD_MS;
        switch (currentState)
        {
        case SelectNote:
            handleNoteChange(currentTimeMs);
            break;
        case PreviewPlayNote:
            handleNoteChange(currentTimeMs);
            if (currentTimeMs - lastNotePlayedMs >= durations[currentDurationIndex])
            {
                stopNote();
                currentState = SelectNote;
            }
            break;
        case SaveNoteToMelody:
            stopNote();
            currentNoteIndex = -1;
            currentDurationIndex = 0;
            currentState = SelectNote;
            encoderReset();
            break;
        case PlayMelody:
            // Handle play melody logic
            break;
        case StopMelody:
            // Handle stop melody logic
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
