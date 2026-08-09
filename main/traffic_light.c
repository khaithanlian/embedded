#include <stdio.h>

#include "traffic_light.h"
#include "led.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define RED_DURATION_MS          3000U
#define RED_YELLOW_DURATION_MS   1000U
#define GREEN_DURATION_MS        3000U
#define YELLOW_DURATION_MS       1000U

static traffic_state_t current_state;

void traffic_light_init(void)
{
    led_init();
    current_state = TRAFFIC_RED;
}

void traffic_light_run(void)
{
    switch (current_state)
    {
        case TRAFFIC_RED:
            printf("STATE: RED\n");
            led_set(LED_RED);
            vTaskDelay(pdMS_TO_TICKS(RED_DURATION_MS));
            current_state = TRAFFIC_RED_YELLOW;
            break;

        case TRAFFIC_RED_YELLOW:
            printf("STATE: RED + YELLOW\n");
            led_set(LED_RED | LED_YELLOW);
            vTaskDelay(pdMS_TO_TICKS(RED_YELLOW_DURATION_MS));
            current_state = TRAFFIC_GREEN;
            break;

        case TRAFFIC_GREEN:
            printf("STATE: GREEN\n");
            led_set(LED_GREEN);
            vTaskDelay(pdMS_TO_TICKS(GREEN_DURATION_MS));
            current_state = TRAFFIC_YELLOW;
            break;

        case TRAFFIC_YELLOW:
            printf("STATE: YELLOW\n");
            led_set(LED_YELLOW);
            vTaskDelay(pdMS_TO_TICKS(YELLOW_DURATION_MS));
            current_state = TRAFFIC_RED;
            break;

        default:
            printf("STATE: INVALID\n");
            current_state = TRAFFIC_RED;
            break;
    }
}