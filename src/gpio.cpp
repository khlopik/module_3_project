#include "config.hpp"

void gpioInit()
{
    gpio_config_t servoPinConfig {};
    servoPinConfig.pin_bit_mask = (1ULL << pin::servo);
    servoPinConfig.mode = GPIO_MODE_OUTPUT;
    servoPinConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    servoPinConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    servoPinConfig.intr_type = GPIO_INTR_DISABLE;
    
    gpio_config(&servoPinConfig);
}