#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

void AccelInit();
void AccelDeinit();
void AccelGetData(SensorData* data);
