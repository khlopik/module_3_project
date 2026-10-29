#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/pulse_cnt.h"

namespace pin {
    constexpr gpio_num_t servo = GPIO_NUM_12;
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
    constexpr gpio_num_t pin = GPIO_NUM_11;
    constexpr ledc_channel_t channel = LEDC_CHANNEL_1;
    constexpr ledc_timer_t timer = LEDC_TIMER_1;
    constexpr ledc_timer_bit_t resolutionBits = LEDC_TIMER_10_BIT;
}

namespace encoder {
    constexpr gpio_num_t pinA = GPIO_NUM_5;
    constexpr gpio_num_t pinB = GPIO_NUM_6;
    constexpr uint16_t maxGlitchNs = 1000;
}

namespace superloop {
    constexpr uint32_t yieldDelayMs = 10;
}

#endif // CONFIG_HPP