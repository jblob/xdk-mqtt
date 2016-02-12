#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

void EnvSensorInit(void);
void EnvSensorDeinit(void);
void EnvSensorGetData(SensorData* data);
