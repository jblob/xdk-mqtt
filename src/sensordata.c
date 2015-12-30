#include <stdio.h>
#include "sensordata.h"

extern void SensorInit(SensorInitializer init, void* handle, const char* label)
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
