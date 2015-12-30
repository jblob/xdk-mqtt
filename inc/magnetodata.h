#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

extern void MagnetoInit();
extern void MagnetoDeinit();
extern void MagnetoGetData(SensorData* data);
