#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static int shared_counter = 0;

static SemaphoreHandle_t counter_mutex;

static void counter_task_1(void *pvParameters)
{
    for (int i = 0; i < 100; i++)
    {
        xSemaphoreTake(counter_mutex, portMAX_DELAY);

        shared_counter++;

        printf(
            "Task 1: counter = %d\n",
            shared_counter
        );

        xSemaphoreGive(counter_mutex);

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL);
}

static void counter_task_2(void *pvParameters)
{
    for (int i = 0; i < 100; i++)
    {
        xSemaphoreTake(counter_mutex, portMAX_DELAY);

        shared_counter++;

        printf(
            "Task 2: counter = %d\n",
            shared_counter
        );

        xSemaphoreGive(counter_mutex);

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL);
}

void shared_counter_demo_start(void)
{
    counter_mutex = xSemaphoreCreateMutex();

    xTaskCreate(
        counter_task_1,
        "CounterTask1",
        2048,
        NULL,
        5,
        NULL
    );

    xTaskCreate(
        counter_task_2,
        "CounterTask2",
        2048,
        NULL,
        5,
        NULL
    );
}