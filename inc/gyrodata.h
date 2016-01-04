#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

void GyroInit();
void GyroDeinit();
void GyroGetData(SensorData* data);
