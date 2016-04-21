#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

void AccelInit(void);
void AccelDeinit(void);
void AccelGetData(SensorData* data);
