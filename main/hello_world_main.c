#include "led.h"
#include "traffic_light.h"

void app_main(void)
{
    traffic_light_init();

    while (1)
    {
        traffic_light_run();
    }
}

