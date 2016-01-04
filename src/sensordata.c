#include <stdio.h>
#include "sensordata.h"

void SensorInit(SensorInitializer init, void* handle, const char* label)
{
    if(SENSOR_SUCCESS == init(handle))
    {
        printf("%s initialization Success\n\r", label);
    }
    else
    {
        printf("%s initialization FAILED\n\r", label);
    }
}

void SensorDataClear(SensorData* data)
{
    data->numMeas = 0;
    for(int i = 0; i < MAX_MEASUREMENTS; ++i)
    {
        data->meas[i].name[0]  = '\0';
        data->meas[i].value[0] = '\0';
    }
}
