#pragma once

#include "sensorTypes.h"

typedef struct
{
    char name[20];
    char value[30];
} NameValue;

typedef struct
{
    char meaning[20];
    NameValue meas[5];
    uint32_t numMeas;
} SensorData;

typedef void(*SensorGetter)(SensorData* data);
typedef sensor_errorType_t(*SensorInitializer)(void* handle);

extern SensorGetter sensors[];
extern int enabledSensors[];
extern void SensorInit(SensorInitializer init, void* handle, const char* label);
