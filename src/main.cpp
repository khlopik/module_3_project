#include "config.hpp"
#include "gpio.hpp"
#include "servo.hpp"
#include "buzzer.hpp"
#include "encoder.hpp"
#include "melody.hpp"
#include "duration_leds.hpp"
#include "digits.hpp"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

namespace
{
    int currentNoteIndex = -1; // -1 indicates no note is currently selected
    uint16_t currentDurationIndex = 0;
    uint32_t lastNotePlayedMs = 0;

    uint16_t getAngleForNoteIndex(uint16_t noteIndex)
    {
        return noteIndex * servoConfig::maxAngle / (tone::count - 1);
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

    uint8_t activeDigit = 0;
    uint8_t digitPattern[] = {
        0b00000000,
        0b00000000,
    };

    void setDigitsPattern(uint8_t number)
    {
        const uint8_t tens = number / 10;
        const uint8_t seconds = number % 10;
        digitPattern[0] = digitDisplayConfig::digits[tens];
        digitPattern[1] = digitDisplayConfig::digits[seconds];
    }

    void handleButtonPress()
    {
        if (gpio::consumePlayMelodyRequest())
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

        if (gpio::consumeResetMelodyRequest())
        {
            currentMelody.clearMelody();
            currentState = SelectNote;
            currentNoteIndex = -1;
            currentDurationIndex = 0;
            durationLeds.showDuration(currentDurationIndex);
            setDigitsPattern(0);
            return;
        }

        if (gpio::consumeChangeDurationRequest())
        {
            if (currentState != SelectNote && currentState != PreviewPlayNote)
            {
                return;
            }
            currentDurationIndex = (currentDurationIndex + 1) % durationCount;
            durationLeds.showDuration(currentDurationIndex);
            ESP_LOGI("DURATION", "New duration index: %d", currentDurationIndex);
        }

        if (encoder::consumeAddNoteRequest())
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
        int count = encoder::getEncoderCount();

        const int newNoteIndex = ((count % tone::count) + tone::count) % tone::count;

        if (newNoteIndex != currentNoteIndex)
        {
            ESP_LOGI("ENCODER", "newNoteIndex: %d", newNoteIndex);
            currentNoteIndex = newNoteIndex;
            uint16_t frequency = tone::tones[currentNoteIndex];
            uint16_t angle = getAngleForNoteIndex(currentNoteIndex);
            ESP_LOGI("SERVO", "angle: %d", angle);
            servo::setAngle(angle);
            if (frequency == tone::rest)
            {
                buzzer::stopNote();
            }
            else
            {
                buzzer::playNote(frequency);
            }

            lastNotePlayedMs = currentTimeMs;
            currentState = PreviewPlayNote;
        }
    }

    void handleDigitsUpdate()
    {
        if (digits::consumeSwitchRequest())
        {
            digits::turnOff();
            digits::showSegments(digitDisplayConfig::digits[activeDigit]);
            digits::activateDigit(activeDigit);
            activeDigit = (activeDigit + 1) % digitDisplayConfig::activatorPinCount;
        }
    }

    void setup()
    {
        gpio::init();
        encoder::init();
        servo::init();
        buzzer::init();
        digits::init();
        setDigitsPattern(0);
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
                    buzzer::stopNote();
                    currentState = SelectNote;
                }
            }
            break;
            case SaveNoteToMelody:
            {
                buzzer::stopNote();
                const uint8_t noteCount = currentMelody.addNote(tone::tones[currentNoteIndex], durationConfigs[currentDurationIndex].durationMs);
                setDigitsPattern(noteCount);
                currentNoteIndex = -1;
                currentDurationIndex = 0;
                durationLeds.showDuration(currentDurationIndex);
                currentState = SelectNote;
                encoder::reset();
            }
            break;
            case PlayMelody:
            {
                const uint8_t notesLeft = currentMelody.playMelody(currentTimeMs);
                setDigitsPattern(notesLeft);
                if (notesLeft == 0)
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

            handleDigitsUpdate();

            vTaskDelay(pdMS_TO_TICKS(superloopConfig::yieldDelayMs));
        }
    }
} // namespace

extern "C" void app_main()
{
    setup();
    loop();
}
