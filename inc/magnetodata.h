#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

void MagnetoInit();
void MagnetoDeinit();
void MagnetoGetData(SensorData* data);
