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


// Below are Traffic lights Examples

// #include "led.h"
// #include "traffic_light.h"

// void app_main(void)
// {
//     traffic_light_init();

//     while (1)
//     {
//         traffic_light_run();
//     }
// }

