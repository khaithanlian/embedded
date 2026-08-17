#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

typedef enum
{
    BUTTON_EVENT_SINGLE_CLICK,
    BUTTON_EVENT_SHORT_PRESS,
    BUTTON_EVENT_LONG_PRESS,
    BUTTON_EVENT_TRIPLE_CLICK
} button_event_t;

typedef void (*button_event_callback_t)(button_event_t event);

void button_init(button_event_callback_t callback);

#endif