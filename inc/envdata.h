#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

void EnvSensorInit();
void EnvSensorDeinit();
void EnvSensorGetData(SensorData* data);
