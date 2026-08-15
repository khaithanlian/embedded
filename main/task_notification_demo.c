#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "task_notification_demo.h"

static TaskHandle_t controller_task_handle = NULL;


static void controller_task(void *pvParameters)
{
    uint32_t notification_count;
    while (1)
    {
        printf("Controller: waiting for notification...\n");

        // ulTaskNotifyTake(
        //     pdTRUE,
        //     portMAX_DELAY
        // );

        notification_count = ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        printf("Controller: received %lu notifications\n",
            (unsigned long)notification_count);

        // printf("Controller: notification received!\n");
    }
}


static void notifier_task(void *pvParameters)
{
    while (1)
    {
        // vTaskDelay(pdMS_TO_TICKS(2000));

        // printf("Notifier: sending notification\n");

        // xTaskNotifyGive(controller_task_handle);
         vTaskDelay(pdMS_TO_TICKS(3000));

        printf("Notifier: sending notification 1\n");
        xTaskNotifyGive(controller_task_handle);

        printf("Notifier: sending notification 2\n");
        xTaskNotifyGive(controller_task_handle);

        printf("Notifier: sending notification 3\n");
        xTaskNotifyGive(controller_task_handle);
    }
}


void task_notification_demo_start(void)
{
    xTaskCreate(
        controller_task,
        "NotificationController",
        2048,
        NULL,
        5,
        &controller_task_handle
    );

    xTaskCreate(
        notifier_task,
        "NotifierTask",
        2048,
        NULL,
        5,
        NULL
    );
}