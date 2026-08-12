#include "led.h"
#include "board.h"

#include "driver/gpio.h"

static void set_all_off(void)
{
    gpio_set_level(RED_LED, 0);
    gpio_set_level(YELLOW_LED, 0);
    gpio_set_level(GREEN_LED, 0);
}

void led_init(void)
{
    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(YELLOW_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(GREEN_LED, GPIO_MODE_OUTPUT);

    set_all_off();
}

void led_set(led_mask_t mask)
{
    gpio_set_level(RED_LED,    (mask & LED_RED)    != 0);
    gpio_set_level(YELLOW_LED, (mask & LED_YELLOW) != 0);
    gpio_set_level(GREEN_LED,  (mask & LED_GREEN)  != 0);
}

//new setup for freeRTOS testing
void led_red_on(void)
{
    gpio_set_level(RED_LED, 1);
}

void led_red_off(void)
{
    gpio_set_level(RED_LED, 0);
}

void led_yellow_on(void)
{
    gpio_set_level(YELLOW_LED, 1);
}

void led_yellow_off(void)
{
    gpio_set_level(YELLOW_LED, 0);
}