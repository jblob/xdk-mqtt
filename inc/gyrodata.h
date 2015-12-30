#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

extern void GyroInit();
extern void GyroDeinit();
extern void GyroGetData(SensorData* data);
