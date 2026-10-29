#include "config.hpp"
#include "driver/pulse_cnt.h"
#include "esp_log.h"

pcnt_unit_handle_t pcnt_unit = NULL;
pcnt_channel_handle_t pcnt_channel_0 = NULL;
pcnt_channel_handle_t pcnt_channel_1 = NULL;

void encoderInit()
{
    pcnt_unit_config_t unit_config = {};
    unit_config.high_limit = 10000;
    unit_config.low_limit = -10000;
    unit_config.flags.accum_count = true;

    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    pcnt_chan_config_t chan_config_0 = {};

    chan_config_0.edge_gpio_num = encoder::pinA;
    chan_config_0.level_gpio_num = encoder::pinB;

    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config_0, &pcnt_channel_0));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_channel_0,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE
    ));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_channel_0,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE
    ));

    pcnt_chan_config_t chan_config_1 = {};
    chan_config_1.edge_gpio_num = encoder::pinB;
    chan_config_1.level_gpio_num = encoder::pinA;

    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config_1, &pcnt_channel_1));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_channel_1,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE
    ));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_channel_1,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE
    ));

    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = encoder::maxGlitchNs,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

    ESP_LOGI("ENCODER", "PCNT X4 Quadrature Mode Initialized");
}

int getEncoderCount()
{   
    int value;
    ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &value));
    return value / 4; // Divide by 4 to get the actual count in quadrature mode
}

void encoderReset()
{
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
}