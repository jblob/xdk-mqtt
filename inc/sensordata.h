#pragma once

#include <stdbool.h>
#include "sensorTypes.h"

// Constants
#define MAX_MEASUREMENTS (  5 )
#define NUM_SENSORS      (  5 )
#define SENSOR_NAME_SZ   ( 20 )
#define SENSOR_VALUE_SZ  ( 30 )

// Types
typedef struct
{
    char name[SENSOR_NAME_SZ];
    char value[SENSOR_VALUE_SZ];
} NameValue;

typedef struct
{
    NameValue meas[MAX_MEASUREMENTS];
    uint32_t numMeas;
} SensorData;

typedef void(*SensorGetter)(SensorData* data);
typedef sensor_errorType_t(*SensorInitializer)(void* handle);

// Global Variables

// Defined in main.c
extern SensorGetter sensors[];
extern bool enabledSensors[];

// Prototypes
void SensorInit(SensorInitializer init, void* handle, const char* label);
void SensorDataClear(SensorData* data);
