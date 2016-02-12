#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

void LightInit(void);
void LightDeinit(void);
void LightGetData(SensorData* data);
