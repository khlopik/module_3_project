#pragma once

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/pulse_cnt.h"

namespace pin {
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
}

namespace servo {
    constexpr gpio_num_t pin = pin::servo;
    constexpr ledc_channel_t channel = LEDC_CHANNEL_0;
    constexpr ledc_timer_t timer = LEDC_TIMER_0;
    constexpr uint32_t frequencyHz = 50;
    constexpr ledc_timer_bit_t resolutionBits = LEDC_TIMER_13_BIT;
    constexpr uint32_t minPulseWidthUs = 500;
    constexpr uint32_t maxPulseWidthUs = 2500;
    constexpr uint32_t periodUs = 20000;
    constexpr uint16_t maxAngle = 180;
    
}

namespace buzzer {
    constexpr gpio_num_t pin = pin::buzzer;
    constexpr ledc_channel_t channel = LEDC_CHANNEL_1;
    constexpr ledc_timer_t timer = LEDC_TIMER_1;
    constexpr ledc_timer_bit_t resolutionBits = LEDC_TIMER_10_BIT;
    constexpr uint32_t timerFrequencyHz = 440;
    constexpr uint16_t dutyCycle = 512; // 50% duty cycle for square wave
}

namespace encoder {
    constexpr gpio_num_t pinA = pin::encoderA;
    constexpr gpio_num_t pinB = pin::encoderB;
    constexpr uint16_t maxGlitchNs = 1000;
}

namespace melody {
    constexpr uint8_t maxNotes = 99;
    constexpr uint16_t restBetweenNotesMs = 50;
}

namespace superloop {
    constexpr uint32_t yieldDelayMs = 10;
}
