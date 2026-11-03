#include "digits.hpp"
#include "config.hpp"
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"

namespace
{
    constexpr uint64_t getPinBitMask(const gpio_num_t *pins, size_t count)
    {
        uint64_t mask = 0;
        for (size_t i = 0; i < count; ++i)
        {
            mask |= (1ULL << pins[i]);
        }
        return mask;
    }

    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
    volatile bool digitSwitchPending = false;
    gptimer_handle_t digitSwitchTimer;

    bool IRAM_ATTR digitSwitchTimerCallback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
    {
        portENTER_CRITICAL_ISR(&timerMux);
        digitSwitchPending = true;
        portEXIT_CRITICAL_ISR(&timerMux);
        return false;
    }

    const gptimer_config_t timerConfig = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Select the default clock source
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1 * 1000 * 1000,
        .intr_priority = 0,
        .flags = {
            .intr_shared = false,
            .allow_pd = false,
        },
    };

    void configTimer()
    {
        gptimer_alarm_config_t digitSwitchTimerAlarmConfig = {};
        digitSwitchTimerAlarmConfig.alarm_count = digitDisplayConfig::scanIntervalUs;
        digitSwitchTimerAlarmConfig.flags.auto_reload_on_alarm = true;
        digitSwitchTimerAlarmConfig.reload_count = 0;

        gptimer_event_callbacks_t digitSwitchTimerCallbacks = {
            .on_alarm = digitSwitchTimerCallback,
        };

        ESP_ERROR_CHECK(gptimer_new_timer(&timerConfig, &digitSwitchTimer));
        ESP_ERROR_CHECK(gptimer_set_alarm_action(digitSwitchTimer, &digitSwitchTimerAlarmConfig));
        ESP_ERROR_CHECK(gptimer_register_event_callbacks(digitSwitchTimer, &digitSwitchTimerCallbacks, NULL));
        ESP_ERROR_CHECK(gptimer_enable(digitSwitchTimer));
        ESP_ERROR_CHECK(gptimer_start(digitSwitchTimer));
    }
} // namespace

namespace digits
{
    void init()
    {
        gpio_config_t config = {};
        config.pin_bit_mask = getPinBitMask(digitDisplayConfig::pins, digitDisplayConfig::pinCount);
        config.mode = GPIO_MODE_OUTPUT;
        config.pull_up_en = GPIO_PULLUP_DISABLE;
        config.pull_down_en = GPIO_PULLDOWN_DISABLE;
        config.intr_type = GPIO_INTR_DISABLE;

        gpio_config(&config);

        config.pin_bit_mask = getPinBitMask(digitDisplayConfig::activatorPins, digitDisplayConfig::activatorPinCount);
        gpio_config(&config);

        configTimer();
    }

    void activateDigit(uint8_t digit)
    {
        gpio_set_level(digitDisplayConfig::activatorPins[digit], digitDisplayConfig::activatorActiveLevel);
    }

    void showSegments(uint8_t mask)
    {
        for (size_t i = 0; i < digitDisplayConfig::pinCount; ++i)
        {
            const bool on = mask & (1u << i);
            gpio_set_level(digitDisplayConfig::pins[i], on ? digitDisplayConfig::segmentActiveLevel : digitDisplayConfig::segmentInactiveLevel);
        }
    }

    bool consumeSwitchRequest()
    {
        portENTER_CRITICAL(&timerMux);
        const bool digitSwitch = digitSwitchPending;
        digitSwitchPending = false;
        portEXIT_CRITICAL(&timerMux);

        return digitSwitch;
    }

    void turnOff()
    {
        for (size_t i = 0; i < digitDisplayConfig::activatorPinCount; ++i)
        {
            gpio_set_level(digitDisplayConfig::activatorPins[i], digitDisplayConfig::activatorInactiveLevel);
        }
    }
}