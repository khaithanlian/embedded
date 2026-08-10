#ifndef COUNTER_TASK_H
#define COUNTER_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct
{
    int task_number;
    int delay_ms;
} counter_task_config_t;

void counter_task_create(
    const counter_task_config_t *config,
    TaskHandle_t *task_handle
);

#endif