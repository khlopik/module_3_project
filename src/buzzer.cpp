#include "buzzer.hpp"
#include "config.hpp"
#include <driver/ledc.h>

void buzzerInit()
{
    ledc_timer_config_t timerConfig {};
    timerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConfig.duty_resolution = buzzer::resolutionBits;
    timerConfig.timer_num = buzzer::timer;
    timerConfig.freq_hz = 440; // ToDo: replace with const
    timerConfig.clk_cfg = LEDC_AUTO_CLK;

    ledc_timer_config(&timerConfig);

    ledc_channel_config_t channelConfig {};
    channelConfig.gpio_num = buzzer::pin;
    channelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    channelConfig.channel = buzzer::channel;
    // channelConfig.intr_type = LEDC_INTR_DISABLE; // ToDo: Remove if works
    channelConfig.timer_sel = buzzer::timer;
    channelConfig.duty = 0;
    channelConfig.hpoint = 0;
    channelConfig.flags.output_invert = 0;

    ledc_channel_config(&channelConfig);
}

void playNote(uint16_t frequency, uint16_t durationMs)
{
    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_1, frequency);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 512);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    // Implement the logic to play a note using a buzzer
    // This is a placeholder implementation
    // You can use PWM or other methods to generate sound
}

void stopNote()
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}