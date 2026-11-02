#pragma once

#include "melody.hpp"
#include "config.hpp"
#include "driver/gpio.h"

class DurationLeds
{
private:
    static constexpr uint32_t ledPinActiveLevel = 1;
    static constexpr uint32_t ledPinInactiveLevel = 0;
    gpio_num_t mLedPins[4] = {pin::led1, pin::led2, pin::led3, pin::led4};
    uint8_t mLedCount = sizeof(mLedPins) / sizeof(mLedPins[0]);

public:
    uint8_t ledCountFor(uint16_t durationMs)
    {
        for (const auto &config : durationConfigs)
        {
            if (config.durationMs == durationMs)
            {
                return config.ledCount;
            }
        }
        return 0;
    }

    void showDuration(size_t durationIndex)
    {
        for (size_t i = 0; i < mLedCount; i++)
        {
            const bool on = i < durationConfigs[durationIndex].ledCount;
            gpio_set_level(mLedPins[i], on ? ledPinActiveLevel : ledPinInactiveLevel);
        }
    }
};
