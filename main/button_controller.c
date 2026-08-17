#include "button_controller.h"
#include "button.h"
#include "led.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdio.h>


static TaskHandle_t controller_task_handle = NULL;


/* -----------------------------------------------------------
 * Controller Task
 * ----------------------------------------------------------- */

static void controller_task(void *pvParameters)
{
    uint32_t notification_value;


    while (1)
    {
        printf(
            "Controller: waiting for button event...\n"
        );


        /*
         * Wait until Button Task sends an event.
         *
         * The notification value itself contains
         * the button event number.
         */

        xTaskNotifyWait(
            0,
            0xFFFFFFFFUL,
            &notification_value,
            portMAX_DELAY
        );


        button_event_t event =
            (button_event_t)notification_value;


        switch (event)
        {
            /*
             * ------------------------------------------------
             * SINGLE CLICK
             * ------------------------------------------------
             */

            case BUTTON_EVENT_SINGLE_CLICK:

                printf(
                    "Controller: SINGLE CLICK\n"
                );

                led_set(LED_RED);

                vTaskDelay(
                    pdMS_TO_TICKS(500)
                );

                led_set(LED_OFF);

                break;


            /*
             * ------------------------------------------------
             * SHORT PRESS
             * ------------------------------------------------
             */

            case BUTTON_EVENT_SHORT_PRESS:

                printf(
                    "Controller: SHORT PRESS\n"
                );

                led_set(LED_YELLOW);

                vTaskDelay(
                    pdMS_TO_TICKS(500)
                );

                led_set(LED_OFF);

                break;


            /*
             * ------------------------------------------------
             * LONG PRESS
             * ------------------------------------------------
             */

            case BUTTON_EVENT_LONG_PRESS:

                printf(
                    "Controller: LONG PRESS\n"
                );

                /*
                 * Yellow remains ON.
                 */

                led_set(LED_YELLOW);

                break;


            /*
             * ------------------------------------------------
             * TRIPLE CLICK
             * ------------------------------------------------
             */

            case BUTTON_EVENT_TRIPLE_CLICK:

                printf(
                    "Controller: TRIPLE CLICK\n"
                );


                for (int i = 0; i < 3; i++)
                {
                    led_set(LED_GREEN);

                    vTaskDelay(
                        pdMS_TO_TICKS(300)
                    );

                    led_set(LED_OFF);

                    vTaskDelay(
                        pdMS_TO_TICKS(300)
                    );
                }

                break;


            default:

                printf(
                    "Controller: UNKNOWN EVENT = %lu\n",
                    (unsigned long)notification_value
                );

                led_set(LED_OFF);

                break;
        }
    }
}


/* -----------------------------------------------------------
 * Button Event Callback
 * ----------------------------------------------------------- */

static void button_event_callback(button_event_t event)
{
    printf(
        "Controller callback: event = %d\n",
        event
    );


    /*
     * Send the actual event number through
     * the Controller Task's notification.
     */

    xTaskNotify(
        controller_task_handle,

        (uint32_t)event,

        eSetValueWithOverwrite
    );
}


/* -----------------------------------------------------------
 * Start Controller
 * ----------------------------------------------------------- */

void button_controller_start(void)
{
    xTaskCreate(
        controller_task,
        "ButtonController",
        2048,
        NULL,
        5,
        &controller_task_handle
    );


    button_init(
        button_event_callback
    );
}