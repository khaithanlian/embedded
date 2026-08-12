
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "led.h"

static SemaphoreHandle_t led_mutex;

static void red_task(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(led_mutex, portMAX_DELAY);

        led_set(LED_RED);

        xSemaphoreGive(led_mutex);

        vTaskDelay(pdMS_TO_TICKS(500));

        xSemaphoreTake(led_mutex, portMAX_DELAY);

        led_set(LED_OFF);

        xSemaphoreGive(led_mutex);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
static void yellow_task(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(led_mutex, portMAX_DELAY);

        led_set(LED_YELLOW);

        xSemaphoreGive(led_mutex);

        vTaskDelay(pdMS_TO_TICKS(1000));

        xSemaphoreTake(led_mutex, portMAX_DELAY);

        led_set(LED_OFF);

        xSemaphoreGive(led_mutex);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void timing_demo_start(void)
{
    led_mutex = xSemaphoreCreateMutex();
    
    xTaskCreate(
        red_task,
        "RedTask",
        2048,
        NULL,
        5,
        NULL
    );

    xTaskCreate(
        yellow_task,
        "YellowTask",
        2048,
        NULL,
        5,
        NULL
    );
}