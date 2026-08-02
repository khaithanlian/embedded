/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_flash.h"
#include "my_functions.h"
#include "led.h"

void app_main(void)
{
    led_init();

    while (1)
    {
        red_on();
        vTaskDelay(pdMS_TO_TICKS(1000));

        yellow_on();
        vTaskDelay(pdMS_TO_TICKS(1000));

        green_on();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


// GPIO I/O test
// #define LED_GPIO GPIO_NUM_4
// #define BUTTON_GPIO GPIO_NUM_36

// void app_main(void)
// {
//     gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
//     gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);

//     gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);

//     while (true)
//     {
//         int button_state = false; 
//         // gpio_get_level(BUTTON_GPIO);

//     printf("Button state is = %d\n", button_state);

//     if(button_state == false){
//         gpio_set_level(LED_GPIO, true);
//         vTaskDelay(pdMS_TO_TICKS(500));
//         gpio_set_level(LED_GPIO, false);
//         vTaskDelay(pdMS_TO_TICKS(500));
//     }else{

//         gpio_set_level(LED_GPIO, false);
//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
//     }
// }

