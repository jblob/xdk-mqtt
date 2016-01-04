#include "accelerometer.h"
#include "xdk_sensors.h"
#include "acceldata.h"

static const char ACCEL_LABEL[] = "BMA280 Accelerometer";

// Static declarations
static void FillAccelData(SensorData* data, accelerometer_xyzData_t* meas)
{
    // Setup SensorData
    data->numMeas = 1;
    snprintf(data->meas[0].name, SENSOR_NAME_SZ, "%s", "acceleration");

    // Setup data
    snprintf(data->meas[0].value,
             SENSOR_VALUE_SZ,
             "{\"x\":%f, \"y\":%f, \"z\":%f}",
             (float)meas->xAxisData/1000.0,
             (float)meas->yAxisData/1000.0,
             (float)meas->zAxisData/1000.0);
}

void AccelInit()
{
    SensorInit(&accelerometerInit,
               xdkAccelerometers_BMA280_Handle,
               ACCEL_LABEL);
}

void AccelDeinit()
{
}

void AccelGetData(SensorData* data)
{
    sensor_errorType_t returnValue = SENSOR_ERROR;
    accelerometer_xyzData_t getaccelData = {0};

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

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
