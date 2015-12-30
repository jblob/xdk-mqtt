#include "gyroscope.h"
#include "xdk_sensors.h"
#include "gyrodata.h"

static const char GYRO_LABEL[] = "BMG160 Gyroscope";

void InitGyroData(SensorData* storage)
{
    storage->numMeas = 1;
    sprintf(storage->meas[0].name, "%s", "gyro");
}

void FillGyroData(SensorData* data, gyro_xyzData_t* meas)
{
    InitGyroData(data);
    sprintf(data->meas[0].value,
            "{\"x\":%f, \"y\":%f, \"z\":%f}",
            (float)meas->xData/1000.0,
            (float)meas->yData/1000.0,
            (float)meas->zData/1000.0);
}

extern void GyroInit()
{
    SensorInit(&gyroscopeInit,
               xdkGyroscope_BMG160_Handle,
               GYRO_LABEL);
}

extern void GyroDeinit()
{
}

extern void GyroGetData(SensorData* data)
{
    sensor_errorType_t returnValue = SENSOR_ERROR;
    gyro_xyzData_t getRawData = {0};
    gyro_xyzData_t getMdegData = {0};

    returnValue = gyroscopeReadXyzValue(xdkGyroscope_BMG160_Handle, &getRawData);
    if(SENSOR_SUCCESS == returnValue)
    {
        printf("\n%s Raw Data : x = %ld, y = %ld, z = %ld\n",
               GYRO_LABEL,
               (long int)getRawData.xData,
               (long int)getRawData.yData,
               (long int)getRawData.zData);

    }
    else
    {
        printf("%s Raw Data read FAILED\n\r", GYRO_LABEL);
    }

    returnValue = gyroscopeReadXyzDegreeValue(xdkGyroscope_BMG160_Handle, &getMdegData);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillGyroData(data, &getMdegData);
        printf("%s Converted Data : x = %ld mDeg, y = %ld mDeg, z = %ld mDeg\n\r",
               GYRO_LABEL,
               (long int)getMdegData.xData,
               (long int)getMdegData.yData,
               (long int)getMdegData.zData);

    }
    else
    {
        printf("%s Converted Data read FAILED\n\r", GYRO_LABEL);
    }
}
