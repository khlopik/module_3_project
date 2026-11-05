#include "config.hpp"
#include "leds.hpp"

namespace leds
{
    void init()
    {
        gpio_config_t ledPinConfig{};
        ledPinConfig.pin_bit_mask = (1ULL << pinConfig::led1) | (1ULL << pinConfig::led2) | (1ULL << pinConfig::led3) | (1ULL << pinConfig::led4);
        ledPinConfig.mode = GPIO_MODE_OUTPUT;
        ledPinConfig.pull_up_en = GPIO_PULLUP_DISABLE;
        ledPinConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
        ledPinConfig.intr_type = GPIO_INTR_DISABLE;

        gpio_config(&ledPinConfig);
    }

    void showDuration(size_t durationIndex)
    {
        for (size_t i = 0; i < ledConfig::count; i++)
        {
            const bool on = i < durationConfig::durationLedConfig[durationIndex].ledCount;
            gpio_set_level(ledConfig::pins[i], on ? ledConfig::pinActiveLevel : ledConfig::pinInactiveLevel);
        }
    }
}