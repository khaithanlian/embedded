#include "led.h"
#include "timing_demo.h"
#include "shared_counter.h"
#include "led_controller.h"

void app_main(void)
{
    led_init();
    // timing_demo_start();
    // shared_counter_demo_start();
    led_controller_start();
}
