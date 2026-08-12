#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "led.h"
#include "led_controller.h"

static QueueHandle_t led_queue;


static void led_controller_task(void *pvParameters)
{
    led_command_t command;

    while (1)
    {
        if (xQueueReceive(
                led_queue,
                &command,
                portMAX_DELAY
            ) == pdTRUE)
        {
            switch (command)
            {
                case LED_COMMAND_OFF:
                    led_set(LED_OFF);
                    break;

                case LED_COMMAND_RED:
                    led_set(LED_RED);
                    break;

                case LED_COMMAND_YELLOW:
                    led_set(LED_YELLOW);
                    break;

                default:
                    led_set(LED_OFF);
                    break;
            }
        }
    }
}


static void red_command_task(void *pvParameters)
{
    led_command_t command;

    while (1)
    {
        command = LED_COMMAND_RED;

        xQueueSend(
            led_queue,
            &command,
            portMAX_DELAY
        );

        vTaskDelay(pdMS_TO_TICKS(1000));

        command = LED_COMMAND_OFF;

        xQueueSend(
            led_queue,
            &command,
            portMAX_DELAY
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


static void yellow_command_task(void *pvParameters)
{
    led_command_t command;

    while (1)
    {
        command = LED_COMMAND_YELLOW;

        xQueueSend(
            led_queue,
            &command,
            portMAX_DELAY
        );

        vTaskDelay(pdMS_TO_TICKS(2000));

        command = LED_COMMAND_OFF;

        xQueueSend(
            led_queue,
            &command,
            portMAX_DELAY
        );

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}


void led_controller_start(void)
{
    led_queue = xQueueCreate(
        10,
        sizeof(led_command_t)
    );

    xTaskCreate(
        led_controller_task,
        "LEDController",
        2048,
        NULL,
        5,
        NULL
    );

    xTaskCreate(
        red_command_task,
        "RedCommand",
        2048,
        NULL,
        5,
        NULL
    );

    xTaskCreate(
        yellow_command_task,
        "YellowCommand",
        2048,
        NULL,
        5,
        NULL
    );
}