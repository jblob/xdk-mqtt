#include "gyroscope.h"
#include "xdk_sensors.h"
#include "gyrodata.h"

static const char GYRO_LABEL[] = "BMG160 Gyroscope";

static void FillGyroData(SensorData* data, gyro_xyzData_t* meas)
{
    data->numMeas = 1;
    snprintf(data->meas[0].name, SENSOR_NAME_SZ, "%s", "gyro");

    snprintf(data->meas[0].value,
             SENSOR_VALUE_SZ,
             "{\"x\":%f, \"y\":%f, \"z\":%f}",
             (float)meas->xData/1000.0,
             (float)meas->yData/1000.0,
             (float)meas->zData/1000.0);
}

void GyroInit()
{
    SensorInit(&gyroscopeInit,
               xdkGyroscope_BMG160_Handle,
               GYRO_LABEL);
}

void GyroDeinit()
{
}

void GyroGetData(SensorData* data)
{
    sensor_errorType_t returnValue = SENSOR_ERROR;
    gyro_xyzData_t getRawData = {0};
    gyro_xyzData_t getMdegData = {0};

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

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
