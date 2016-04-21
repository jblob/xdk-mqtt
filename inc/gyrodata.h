#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

void GyroInit(void);
void GyroDeinit(void);
void GyroGetData(SensorData* data);
