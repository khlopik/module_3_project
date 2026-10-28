#include "servo.hpp"
#include "config.hpp"
#include <driver/ledc.h>
#include "esp_log.h"

void servoInit()
{
    ledc_timer_config_t timerConfig {};
    timerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConfig.duty_resolution = servo::resolutionBits;
    timerConfig.timer_num = servo::timer;
    timerConfig.freq_hz = servo::frequencyHz;
    timerConfig.clk_cfg = LEDC_AUTO_CLK;

    ledc_timer_config(&timerConfig);

    ledc_channel_config_t channelConfig {};
    channelConfig.gpio_num = servo::pin;
    channelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    channelConfig.channel = servo::channel;
    channelConfig.intr_type = LEDC_INTR_DISABLE;
    channelConfig.timer_sel = servo::timer;
    channelConfig.duty = 0;
    channelConfig.hpoint = 0;
    channelConfig.flags.output_invert = 0;

    ledc_channel_config(&channelConfig);
}

void servoSetAngle(int16_t angle)
{
    angle = angle > 180 ? 180 : (angle < 0 ? 0 : angle);

    uint32_t pulseWidthUs = servo::minPulseWidthUs + ((servo::maxPulseWidthUs - servo::minPulseWidthUs) * angle) / 180;
    uint32_t duty = (pulseWidthUs * (1 << servo::resolutionBits)) / servo::periodUs;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, servo::channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, servo::channel);
    ESP_LOGI("servo.cpp", "Angle: %d° → Pulse: %d us → Duty: %d\n", angle, pulseWidthUs, duty);
}