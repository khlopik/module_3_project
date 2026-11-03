#include "duration_leds.hpp"
#include "melody.hpp"

uint8_t DurationLeds::ledCountFor(uint16_t durationMs)
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

void DurationLeds::showDuration(size_t durationIndex)
{
    for (size_t i = 0; i < ledConfig::count; i++)
    {
        const bool on = i < durationConfigs[durationIndex].ledCount;
        gpio_set_level(ledConfig::pins[i], on ? ledConfig::pinActiveLevel : ledConfig::pinInactiveLevel);
    }
}