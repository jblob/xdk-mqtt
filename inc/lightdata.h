#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

void LightInit();
void LightDeinit();
void LightGetData(SensorData* data);
