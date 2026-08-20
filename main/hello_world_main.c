#include "led.h"
// #include "timing_demo.h"
// #include "shared_counter.h"
#include "led_controller.h"
// #include "task_notification_demo.h"
#include "button_controller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include <stdio.h>

// void app_main(void)
// {
//     // task_notification_demo_start();
//     led_init();
//     button_controller_start();
//     // timing_demo_start();
//     // shared_counter_demo_start();
//     // led_controller_start();
// }


// SYSTEM / FreeRTOS
//         │
//         ├── xTimerCreate()
//         ├── xTimerStart()
//         └── manages timer
//                  │
//                  ▼
//           calls OUR function
//                  │
//                  ▼
//           timer_callback()


static TimerHandle_t led_timer;

static bool led_state = true;


static void led_timer_callback(TimerHandle_t timer)
{
    led_state = !led_state;

    if (led_state)
    {
        led_set(LED_RED);

        printf("Timer callback: RED ON\n");
    }
    else
    {
        led_set(LED_OFF);

        printf("Timer callback: RED OFF\n");
    }
    xTimerStop(led_timer, 0);
}


void app_main(void)
{
    
    printf("Starting software timer...\n");

    #if 0 
        TIMER conceptually
            xTimerCreate(
            NAME,
            PERIOD,
            REPEAT?,
            TIMER_ID,
            CALLBACK
        );
    #endif

    led_init();
    led_set(LED_RED);

    led_timer = xTimerCreate(
        "LEDTimer",
        pdMS_TO_TICKS(1000),
        pdFALSE,
        NULL,
        led_timer_callback
    );


    if (led_timer == NULL)
    {
        printf("ERROR: Timer creation failed\n");
        return;
    }


    if (xTimerStart(led_timer,0) != pdPASS){
        
        printf("ERROR: Timer start failed\n");
        return;
    }


    printf("Software timer started\n");
}