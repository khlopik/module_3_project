#pragma once

#include <stdint.h>

class DurationLeds
{
public:
    uint8_t getLedCountForDuration(uint16_t durationMs);
    void showDuration(size_t durationIndex);
};

namespace leds
{
    void init();
}