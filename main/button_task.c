#include "button.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include <stdio.h>
#include "board.h"

#define POLL_INTERVAL_MS       20

#define SINGLE_CLICK_MAX_MS    150
#define LONG_PRESS_MS          1500

#define TRIPLE_CLICK_WINDOW_MS 1450


static button_event_callback_t event_callback = NULL;


static void button_task(void *pvParameters)
{
    int last_state = 1;

    TickType_t press_start_time = 0;

    uint32_t click_count = 0;
    TickType_t first_click_time = 0;


    while (1)
    {
        int current_state = gpio_get_level(BUTTON_GPIO);
        //Button Press

        if (last_state == 1 && current_state == 0)
        {
            press_start_time = xTaskGetTickCount();
        }


       // Button Release
        if (last_state == 0 && current_state == 1)
        {
            TickType_t press_duration_ticks =
                xTaskGetTickCount() - press_start_time;

            uint32_t press_duration_ms =
                pdTICKS_TO_MS(press_duration_ticks);

            //LONG PRESS

            if (press_duration_ms >= LONG_PRESS_MS)
            {
                printf(
                    "Button: LONG PRESS (%lu ms)\n",
                    (unsigned long)press_duration_ms
                );

                click_count = 0;

                if (event_callback != NULL)
                {
                    event_callback(
                        BUTTON_EVENT_LONG_PRESS
                    );
                }
            }

            //SHORT PRESS

            else if (press_duration_ms >= SINGLE_CLICK_MAX_MS)
            {
                printf(
                    "Button: SHORT PRESS (%lu ms)\n",
                    (unsigned long)press_duration_ms
                );

                /*
                 * This is not part of a click sequence.
                 */

                click_count = 0;

                if (event_callback != NULL)
                {
                    event_callback(
                        BUTTON_EVENT_SHORT_PRESS
                    );
                }
            }


            //QUICK CLICK

            else
            {
                /*
                 * First quick click.
                 */

                if (click_count == 0)
                {
                    first_click_time = xTaskGetTickCount();
                }

                click_count++;


                uint32_t elapsed_ms =
                    pdTICKS_TO_MS(
                        xTaskGetTickCount() - first_click_time
                    );


                /*
                 * ------------------------------------------------
                 * TRIPLE CLICK
                 * ------------------------------------------------
                 */

                if (
                    click_count == 3 &&
                        elapsed_ms <= TRIPLE_CLICK_WINDOW_MS
                )
                {
                    printf(
                        "Button: TRIPLE CLICK\n"
                    );

                    click_count = 0;

                    if (event_callback != NULL)
                    {
                        event_callback(
                            BUTTON_EVENT_TRIPLE_CLICK
                        );
                    }
                }
            }
        }


        /*
         * ----------------------------------------------------
         * SINGLE CLICK TIMEOUT
         * ----------------------------------------------------
         *
         * If we have one quick click and no second/third
         * click arrives within 1000 ms, it becomes a
         * SINGLE CLICK.
         */

        if (click_count > 0)
        {
            uint32_t elapsed_ms =
                pdTICKS_TO_MS(
                    xTaskGetTickCount() - first_click_time
                );


            if (elapsed_ms > TRIPLE_CLICK_WINDOW_MS)
            {
                if (click_count == 1)
                {
                    printf(
                        "Button: SINGLE CLICK\n"
                    );

                    if (event_callback != NULL)
                    {
                        event_callback(
                            BUTTON_EVENT_SINGLE_CLICK
                        );
                    }
                }
                else
                {
                    /*
                     * Two quick clicks were detected,
                     * but they did not become a triple click.
                     *
                     * For this lesson we treat this as a
                     * single-click outcome.
                     */

                    printf(
                        "Button: %lu quick clicks - no triple click\n",
                        (unsigned long)click_count
                    );

                    if (event_callback != NULL)
                    {
                        event_callback(
                            BUTTON_EVENT_SINGLE_CLICK
                        );
                    }
                }

                click_count = 0;
            }
        }


        last_state = current_state;


        vTaskDelay(
            pdMS_TO_TICKS(POLL_INTERVAL_MS)
        );
    }
}


void button_init(button_event_callback_t callback)
{
    event_callback = callback;


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
        button_task,
        "ButtonTask",
        2048,
        NULL,
        5,
        NULL
    );
}