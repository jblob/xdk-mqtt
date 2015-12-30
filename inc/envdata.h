#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

extern void EnvSensorInit();
extern void EnvSensorDeinit();
extern void EnvSensorGetData(SensorData* data);
