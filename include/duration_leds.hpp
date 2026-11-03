#pragma once

#include <stdint.h>

class DurationLeds
{
public:
    uint8_t ledCountFor(uint16_t durationMs);
    void showDuration(size_t durationIndex);
};
