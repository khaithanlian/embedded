#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "led.h"
#include "led_controller.h"

static QueueHandle_t led_queue;

static void led_controller_task(void *pvParameters)
{
    led_request_t request;

    while (1)
    {
        printf("Controller waiting for command...\n");
        
        if (xQueueReceive(
                led_queue,
                &request,
                portMAX_DELAY
            ) == pdTRUE)
        {   
            printf("Controller received command: %d, duration: %lu ms\n",
            request.command, (unsigned long)request.duration_ms);
            
            switch (request.command)
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

            vTaskDelay(
                pdMS_TO_TICKS(request.duration_ms)
            );

            led_set(LED_OFF);
            printf("Controller processing finished\n");
            
        }
    }
}
static void red_command_task(void *pvParameters)
{
    led_request_t request;

    while (1)
    {
        request.command = LED_COMMAND_RED;
        request.duration_ms = 1000;

        xQueueSend(
            led_queue,
            &request,
            portMAX_DELAY
        );

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
static void yellow_command_task(void *pvParameters)
{
    led_request_t request;

    while (1)
    {
        request.command = LED_COMMAND_YELLOW;
        request.duration_ms = 2000;

        xQueueSend(
            led_queue,
            &request,
            portMAX_DELAY
        );

        vTaskDelay(pdMS_TO_TICKS(4000));
    }
}
void led_controller_start(void)
{
    led_queue = xQueueCreate(
        10,
        sizeof(led_request_t)
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