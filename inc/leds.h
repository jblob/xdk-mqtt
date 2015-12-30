#pragma once

#include "led.h"

static const int RED_LED = 0;
static const int YELLOW_LED = 1;
static const int ORANGE_LED = 2;
static const int NUM_LEDS = 3;

extern int LedInit();
extern int LedDeinit();
extern void LedSet(int led, LED_operations_t op);
