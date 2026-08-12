#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "led.h"

static void red_task(void *pvParameters)
{
    while (1)
    {
        led_set(LED_RED);
        vTaskDelay(pdMS_TO_TICKS(500));

        led_set(LED_OFF);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void yellow_task(void *pvParameters)
{
    while (1)
    {
        led_set(LED_YELLOW);
        vTaskDelay(pdMS_TO_TICKS(1000));

        led_set(LED_OFF);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void timing_demo_start(void)
{
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
//20260812
#include "counter_task.h"

static void controller_task(void *pvParameters)
{
    TaskHandle_t task_handle = (TaskHandle_t)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(5000));

    printf("Suspending Task 1\n");
    vTaskSuspend(task_handle);

    vTaskDelay(pdMS_TO_TICKS(5000));

    printf("Resuming Task 1\n");
    vTaskResume(task_handle);

    vTaskDelete(NULL);
}

void app_main(void)
{
    static const counter_task_config_t task_1_config =
    {
        .task_number = 1,
        .delay_ms = 1000
    };

    static const counter_task_config_t task_2_config =
    {
        .task_number = 2,
        .delay_ms = 2000
    };

    TaskHandle_t task_1_handle = NULL;
    TaskHandle_t task_2_handle = NULL;

    counter_task_create(
        &task_1_config,
        &task_1_handle
    );

    counter_task_create(
        &task_2_config,
        &task_2_handle
    );

    xTaskCreate(
        controller_task,
        "ControllerTask",
        2048,
        (void *)task_1_handle,
        6,
        NULL
    );
}

//20260811Lesson
