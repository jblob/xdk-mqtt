#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

extern void AccelInit();
extern void AccelDeinit();
extern void AccelGetData(SensorData* data);
