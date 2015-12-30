#include "accelerometer.h"
#include "xdk_sensors.h"
#include "acceldata.h"

static const char ACCEL_LABEL[] = "BMA280 Accelerometer";

extern void InitAccelData(SensorData* storage)
{
    storage->numMeas = 1;
    sprintf(storage->meas[0].name, "%s", "acceleration");
}

void FillAccelData(SensorData* data, accelerometer_xyzData_t* meas)
{
    InitAccelData(data);
    sprintf(data->meas[0].value,
            "{\"x\":%f, \"y\":%f, \"z\":%f}",
            (float)meas->xAxisData/1000.0,
            (float)meas->yAxisData/1000.0,
            (float)meas->zAxisData/1000.0);
}

extern void AccelInit()
{
    SensorInit(&accelerometerInit,
               xdkAccelerometers_BMA280_Handle,
               ACCEL_LABEL);
}

extern void AccelDeinit()
{
}

extern void AccelGetData(SensorData* data)
{
    sensor_errorType_t returnValue = SENSOR_ERROR;
    accelerometer_xyzData_t getaccelData = {0};

    returnValue = accelerometerReadXyzLsbValue(xdkAccelerometers_BMA280_Handle, &getaccelData);
    if(SENSOR_SUCCESS == returnValue)
    {
        printf("\n%s Raw Data : x = %ld, y = %ld, z = %ld\n",
               ACCEL_LABEL,
               (long int)getaccelData.xAxisData,
               (long int)getaccelData.yAxisData,
               (long int)getaccelData.zAxisData);

    }
    else
    {
        printf("%s Raw Data read FAILED\n\r", ACCEL_LABEL);
    }

    returnValue = accelerometerReadXyzGValue(xdkAccelerometers_BMA280_Handle, &getaccelData);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillAccelData(data, &getaccelData);
        printf("%s Gravity Data : x = %ld mg, y = %ld mg, z = %ld mg\n\r",
               ACCEL_LABEL,
               (long int)getaccelData.xAxisData,
               (long int)getaccelData.yAxisData,
               (long int)getaccelData.zAxisData);

    }
    else
    {
        printf("%s Gravity Data read FAILED\n\r", ACCEL_LABEL);
    }
}
