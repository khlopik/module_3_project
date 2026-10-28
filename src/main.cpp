#include "config.hpp"
#include "gpio.hpp"
#include "servo.hpp"
#include "buzzer.hpp"
#include "encoder.hpp"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

void setup()
{
    gpioInit();
    encoderInit();
    servoInit();
    buzzerInit();
    servoSetAngle(90);
}

void loop()
{
    while (true)
    {
        
        int count = getEncoderCount();
        
        ESP_LOGI("ENCODER", "Position: %d steps", count);
        vTaskDelay(pdMS_TO_TICKS(100));
        // servoSetAngle(0);
        // playNote(440, 500);
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // servoSetAngle(180);
        // stopNote();
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // servoSetAngle(90);
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // vTaskDelay(pdMS_TO_TICKS(superloop::yieldDelayMs));
    }
}

extern "C" void app_main()
{
    setup();
    loop();
}
