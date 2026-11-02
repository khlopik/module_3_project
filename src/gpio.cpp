#include "config.hpp"
#include "iot_button.h"
#include "button_gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "driver/rmt_tx.h"

static volatile bool changeDurationPending = false;
static volatile bool playMelodyPending = false;
static volatile bool resetMelodyPending = false;
static portMUX_TYPE buttonMux = portMUX_INITIALIZER_UNLOCKED;

bool consumeChangeDurationRequest()
{
    portENTER_CRITICAL(&buttonMux);
    const bool changeDuration = changeDurationPending;
    changeDurationPending = false;
    portEXIT_CRITICAL(&buttonMux);

    return changeDuration;
}

bool consumePlayMelodyRequest()
{
    portENTER_CRITICAL(&buttonMux);
    const bool playMelody = playMelodyPending;
    playMelodyPending = false;
    portEXIT_CRITICAL(&buttonMux);

    return playMelody;
}

bool consumeResetMelodyRequest()
{
    portENTER_CRITICAL(&buttonMux);
    const bool resetMelody = resetMelodyPending;
    resetMelodyPending = false;
    portEXIT_CRITICAL(&buttonMux);

    return resetMelody;
}

static void onChangeDurationButtonClick(void *arg, void *data)
{
    ESP_LOGI("GPIO", "ChangeDurationButton single click!");
    portENTER_CRITICAL(&buttonMux);
    changeDurationPending = true;
    portEXIT_CRITICAL(&buttonMux);
}

static void onPlayResetButtonPress(void *arg, void *data)
{
    ESP_LOGI("GPIO", "PlayResetButton single click!");
    portENTER_CRITICAL(&buttonMux);
    playMelodyPending = true;
    portEXIT_CRITICAL(&buttonMux);
}

static void onPlayResetButtonLongPress(void *arg, void *data)
{
    ESP_LOGI("GPIO", "PlayResetButton long press!");
    portENTER_CRITICAL(&buttonMux);
    resetMelodyPending = true;
    portEXIT_CRITICAL(&buttonMux);
}

// void configServo()
// {
//     gpio_config_t servoPinConfig{};
//     servoPinConfig.pin_bit_mask = (1ULL << pin::servo);
//     servoPinConfig.mode = GPIO_MODE_OUTPUT;
//     servoPinConfig.pull_up_en = GPIO_PULLUP_DISABLE;
//     servoPinConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
//     servoPinConfig.intr_type = GPIO_INTR_DISABLE;

//     gpio_config(&servoPinConfig);
// }

void configButtons()
{
    button_config_t btnConfig = {};
    btnConfig.long_press_time = 1000;
    btnConfig.short_press_time = 50;

    button_gpio_config_t btnGpioConfig = {};

    // Duration Button configuration
    btnGpioConfig.gpio_num = pin::durationButton;
    btnGpioConfig.active_level = 0;
    btnGpioConfig.disable_pull = false;

    button_handle_t btn = nullptr;
    ESP_ERROR_CHECK(iot_button_new_gpio_device(&btnConfig, &btnGpioConfig, &btn));
    ESP_ERROR_CHECK(iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, NULL, onChangeDurationButtonClick, NULL));

    // Play/Reset Button configuration
    btnGpioConfig.gpio_num = pin::playResetButton;

    ESP_ERROR_CHECK(iot_button_new_gpio_device(&btnConfig, &btnGpioConfig, &btn));
    ESP_ERROR_CHECK(iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, NULL, onPlayResetButtonPress, NULL));
    ESP_ERROR_CHECK(iot_button_register_cb(btn, BUTTON_LONG_PRESS_START, NULL, onPlayResetButtonLongPress, NULL));
}

void configDurationLeds()
{
    gpio_config_t ledPinConfig{};
    ledPinConfig.pin_bit_mask = (1ULL << pin::led1) | (1ULL << pin::led2) | (1ULL << pin::led3) | (1ULL << pin::led4);
    ledPinConfig.mode = GPIO_MODE_OUTPUT;
    ledPinConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    ledPinConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    ledPinConfig.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&ledPinConfig);
}

void gpioInit()
{
    // configServo();
    configDurationLeds();
    configButtons();
}