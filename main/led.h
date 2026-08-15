#ifndef LED_H
#define LED_H

typedef enum
{
    LED_OFF    = 0,
    LED_RED    = 1 << 0,
    LED_YELLOW = 1 << 1,
    LED_GREEN  = 1 << 2
} led_mask_t;

void led_init(void);
void led_set(led_mask_t mask);


// void led_red_on(void);
// void led_red_off(void);

// void led_yellow_on(void);
// void led_yellow_off(void);

#endif