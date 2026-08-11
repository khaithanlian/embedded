#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "counter_task.h"

static void counter_task(void *pvParameters)
{
    const counter_task_config_t *config =
        (const counter_task_config_t *)pvParameters;

    while (1)
    {
        printf(
            "Task %d running\n",
            config->task_number
        );

        vTaskDelay(pdMS_TO_TICKS(config->delay_ms));
    }
}

void counter_task_create(const counter_task_config_t *config, TaskHandle_t *task_handle){
        xTaskCreate(
        counter_task,
        "CounterTask",
        2048,
        (void *)config,
        5,
        task_handle
    );
}