#include <stdint.h>
#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

typedef enum
{
    LED_COMMAND_OFF,
    LED_COMMAND_RED,
    LED_COMMAND_YELLOW
} led_command_t;

typedef struct
{
    led_command_t command;
    uint32_t duration_ms;
} led_request_t;

void led_controller_start(void);

#endif