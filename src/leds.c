#include "leds.h"
#include <stdio.h>
#include <stdint.h>
#include "XdkBoardHandle.h"

static LED_handle_tp leds[] = {NULL, NULL, NULL};

int LedInit(void)
{
    int ret = 0;
    leds[RED_LED] = LED_create(gpioRedLed_Handle, GPIO_STATE_OFF);
    leds[YELLOW_LED] = LED_create(gpioYellowLed_Handle, GPIO_STATE_OFF);
    leds[ORANGE_LED] = LED_create(gpioOrangeLed_Handle, GPIO_STATE_OFF);

    for(int led = 0; led < NUM_LEDS; ++led)
    {
        if(NULL == leds[led])
        {
            ret -= 1;
        }
    }
    return ret;
}

int LedDeinit(void)
{
    int ret = 0;
    for(int ledidx = 0; ledidx < NUM_LEDS; ++ledidx)
    {
        if(NULL != leds[ledidx])
        {
            ret -= LED_delete(leds[ledidx]);
        }
    }
    return ret;
}

void LedSet(int led, LED_operations_t op)
{
    if(led < NUM_LEDS)
    {
        LED_setState(leds[led], op);
    }
}
