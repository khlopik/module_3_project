#pragma once

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/pulse_cnt.h"

namespace pinConfig
{
    constexpr gpio_num_t servo = GPIO_NUM_12;
    constexpr gpio_num_t encoderButton = GPIO_NUM_15;
    constexpr gpio_num_t encoderA = GPIO_NUM_5;
    constexpr gpio_num_t encoderB = GPIO_NUM_6;
    constexpr gpio_num_t buzzer = GPIO_NUM_11;
    constexpr gpio_num_t durationButton = GPIO_NUM_38;
    constexpr gpio_num_t playResetButton = GPIO_NUM_47;
    constexpr gpio_num_t led1 = GPIO_NUM_16;
    constexpr gpio_num_t led2 = GPIO_NUM_17;
    constexpr gpio_num_t led3 = GPIO_NUM_18;
    constexpr gpio_num_t led4 = GPIO_NUM_8;
    constexpr gpio_num_t digitDisplayA = GPIO_NUM_1;
    constexpr gpio_num_t digitDisplayB = GPIO_NUM_2;
    constexpr gpio_num_t digitDisplayC = GPIO_NUM_4;
    constexpr gpio_num_t digitDisplayD = GPIO_NUM_7;
    constexpr gpio_num_t digitDisplayE = GPIO_NUM_9;
    constexpr gpio_num_t digitDisplayF = GPIO_NUM_10;
    constexpr gpio_num_t digitDisplayG = GPIO_NUM_13;
    constexpr gpio_num_t digitActivator1 = GPIO_NUM_14;
    constexpr gpio_num_t digitActivator2 = GPIO_NUM_21;
}

namespace servoConfig
{
    constexpr gpio_num_t pin = pinConfig::servo;
    constexpr ledc_channel_t channel = LEDC_CHANNEL_0;
    constexpr ledc_timer_t timer = LEDC_TIMER_0;
    constexpr uint32_t frequencyHz = 50;
    constexpr ledc_timer_bit_t resolutionBits = LEDC_TIMER_13_BIT;
    constexpr uint32_t minPulseWidthUs = 500;
    constexpr uint32_t maxPulseWidthUs = 2500;
    constexpr uint32_t periodUs = 20000;
    constexpr uint16_t maxAngle = 180;

}

namespace buzzerConfig
{
    constexpr gpio_num_t pin = pinConfig::buzzer;
    constexpr ledc_channel_t channel = LEDC_CHANNEL_1;
    constexpr ledc_timer_t timer = LEDC_TIMER_1;
    constexpr ledc_timer_bit_t resolutionBits = LEDC_TIMER_10_BIT;
    constexpr uint32_t timerFrequencyHz = 440;
    constexpr uint16_t dutyCycle = 512; // 50% duty cycle for square wave
}

namespace encoderConfig
{
    constexpr gpio_num_t pinA = pinConfig::encoderA;
    constexpr gpio_num_t pinB = pinConfig::encoderB;
    constexpr uint16_t maxGlitchNs = 1000;
}

namespace ledConfig
{
    constexpr gpio_num_t pins[4] = {pinConfig::led1, pinConfig::led2, pinConfig::led3, pinConfig::led4};
    constexpr uint8_t count = sizeof(pins) / sizeof(pins[0]);
    constexpr uint32_t pinActiveLevel = 1;
    constexpr uint32_t pinInactiveLevel = 0;
}

namespace digitDisplayConfig
{
    constexpr gpio_num_t pins[] = {
        pinConfig::digitDisplayA, // 0b00000001
        pinConfig::digitDisplayB, // 0b00000010
        pinConfig::digitDisplayC, // 0b00000100
        pinConfig::digitDisplayD, // 0b00001000
        pinConfig::digitDisplayE, // 0b00010000
        pinConfig::digitDisplayF, // 0b00100000
        pinConfig::digitDisplayG, // 0b01000000
    };
    constexpr uint8_t pinCount = sizeof(digitDisplayConfig::pins) / sizeof(digitDisplayConfig::pins[0]);
    constexpr gpio_num_t activatorPins[] = {
        pinConfig::digitActivator1,
        pinConfig::digitActivator2,
    };
    constexpr uint8_t activatorPinCount = sizeof(digitDisplayConfig::activatorPins) / sizeof(digitDisplayConfig::activatorPins[0]);
    constexpr int segmentActiveLevel = 0;
    constexpr int segmentInactiveLevel = 1;
    constexpr int activatorActiveLevel = 0;
    constexpr int activatorInactiveLevel = 1;
    constexpr uint8_t digits[] = {
        0b00111111, // 0 (A, B, C, D, E, F)
        0b00000110, // 1 (B, C)
        0b01011011, // 2 (A, D, E, F, G)
        0b01001111, // 3 (A, C, D, E, G)
        0b01100110, // 4 (B, C, F, G)
        0b01111111, // 5 (A, B, F, G)
        0b01110111, // 6 (A, B, E, F, G)
        0b00000110, // 7 (A, B, C)
        0b01111111, // 8 (A, B, C, D, E, F, G)
        0b01110111, // 9 (A, B, C, D, F, G)
    };
    constexpr uint32_t scanIntervalUs = 10000;
}

namespace melodyConfig
{
    constexpr uint8_t maxNotes = 99;
    constexpr uint16_t restBetweenNotesMs = 50;
}

namespace superloopConfig
{
    constexpr uint32_t yieldDelayMs = 10;
}
