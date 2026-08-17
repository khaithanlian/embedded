#include "led.h"
// #include "timing_demo.h"
// #include "shared_counter.h"
#include "led_controller.h"
#include "task_notification_demo.h"
#include "button_controller.h"

void app_main(void)
{
    // task_notification_demo_start();
    led_init();
    button_controller_start();
    // timing_demo_start();
    // shared_counter_demo_start();
    // led_controller_start();
}
