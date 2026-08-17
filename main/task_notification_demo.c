#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "led.h"
#include "task_notification_demo.h"
#include "board.h"


static TaskHandle_t controller_task_handle = NULL;


/* -----------------------------------------------------------
 * Controller Task
 * ----------------------------------------------------------- */

static void controller_task(void *pvParameters)
{
    while (1)
    {
        printf("Controller: waiting for button event...\n");

        ulTaskNotifyTake(
            pdFALSE,
            portMAX_DELAY
        );

        printf("Controller: button event received!\n");

        led_set(LED_RED);

        vTaskDelay(
            pdMS_TO_TICKS(1000)
        );
        for (int i=0; i<3; i++){
        
            led_set(LED_OFF);
            vTaskDelay(pdMS_TO_TICKS(200));
            led_set(LED_GREEN);
            vTaskDelay(pdMS_TO_TICKS(200));
            led_set(LED_OFF);
        }
        
    }
}


/* -----------------------------------------------------------
 * Button Task
 * ----------------------------------------------------------- */

static void button_task(void *pvParameters)
{
    int last_state = 1;

    while (1)
    {
        int current_state = gpio_get_level(BUTTON_GPIO);

        /*
         * Detect transition:
         *
         * 1 = not pressed
         * 0 = pressed
         *
         * Therefore:
         * 1 -> 0 means button was pressed.
         */

        if (last_state == 1 && current_state == 0)
        {
            printf("Button Task: button pressed\n");

            xTaskNotifyGive(
                controller_task_handle
            );
        }

        last_state = current_state;

        vTaskDelay(
            pdMS_TO_TICKS(10)
        );
    }
}


/* -----------------------------------------------------------
 * Start notification demo
 * ----------------------------------------------------------- */

void task_notification_demo_start(void)
{
    gpio_config_t button_config =
    {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&button_config);


    xTaskCreate(
        controller_task,
        "NotificationController",
        2048,
        NULL,
        5,
        &controller_task_handle
    );


    xTaskCreate(
        button_task,
        "ButtonTask",
        2048,
        NULL,
        5,
        NULL
    );
}