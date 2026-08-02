#include "led.h"
#include "board.h"

#include "driver/gpio.h"

void led_init(void)
{
    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(YELLOW_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(GREEN_LED, GPIO_MODE_OUTPUT);

    all_off();
}

void all_off(void)
{
    gpio_set_level(RED_LED, 0);
    gpio_set_level(YELLOW_LED, 0);
    gpio_set_level(GREEN_LED, 0);
}

void red_on(void)
{
    all_off();
    gpio_set_level(RED_LED, 1);
}

void yellow_on(void)
{
    all_off();
    gpio_set_level(YELLOW_LED, 1);
}

void green_on(void)
{
    all_off();
    gpio_set_level(GREEN_LED, 1);
}