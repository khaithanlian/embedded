#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

typedef enum
{
    LED_COMMAND_OFF,
    LED_COMMAND_RED,
    LED_COMMAND_YELLOW
} led_command_t;

void led_controller_start(void);

#endif