#pragma once

namespace gpio
{
    void init();
    bool consumeChangeDurationRequest();
    bool consumePlayMelodyRequest();
    bool consumeResetMelodyRequest();
} // namespace gpio