#pragma once

#include <stdint.h>
#include <stdio.h>

static const int32_t BUTTONONE = 1;

typedef void(*buttonHandler)(void*, uint32_t);

extern int ButtonOneInit(buttonHandler handler);
extern int IsButtonPressed(void* handle);
extern int IsButtonReleased(void* handle);
