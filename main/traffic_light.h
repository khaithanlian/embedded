#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

typedef enum
{
    TRAFFIC_RED,
    TRAFFIC_RED_YELLOW,
    TRAFFIC_GREEN,
    TRAFFIC_YELLOW
} traffic_state_t;

void traffic_light_init(void);
void traffic_light_run(void);

#endif