#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"

void MagnetoInit(void);
void MagnetoDeinit(void);
void MagnetoGetData(SensorData* data);
