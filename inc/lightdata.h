#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

extern void LightInit();
extern void LightDeinit();
extern void LightGetData(SensorData* data);
