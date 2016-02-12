#include "BCDS_Gyroscope.h"
#include "XdkSensorHandle.h"
#include "gyrodata.h"

static const char GYRO_LABEL[] = "BMG160 Gyroscope";

static void FillGyroData(SensorData* data, Gyroscope_XyzData_T* meas)
{
    data->numMeas = 1;
    snprintf(data->meas[0].name, SENSOR_NAME_SIZE, "%s", "gyro");

    snprintf(data->meas[0].value,
             SENSOR_VALUE_SIZE,
             "{\"x\":%f, \"y\":%f, \"z\":%f}",
             (float)meas->xAxisData/1000.0,
             (float)meas->yAxisData/1000.0,
             (float)meas->zAxisData/1000.0);
}

static Retcode_T GyroPrivateInit(void* handle)
{
    return Gyroscope_init((Gyroscope_HandlePtr_T)handle);
}

void GyroInit(void)
{
    SensorInit(&GyroPrivateInit,
               xdkGyroscope_BMG160_Handle,
               GYRO_LABEL);
}

void GyroDeinit(void)
{
    Gyroscope_deInit(xdkGyroscope_BMG160_Handle);
}

void GyroGetData(SensorData* data)
{
    Retcode_T returnValue = SENSOR_ERROR;
    Gyroscope_XyzData_T getRawData = {0};
    Gyroscope_XyzData_T getMdegData = {0};

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

    returnValue = Gyroscope_readXyzValue(xdkGyroscope_BMG160_Handle, &getRawData);
    if(SENSOR_SUCCESS == returnValue)
    {
        printf("\n%s Raw Data : x = %ld, y = %ld, z = %ld\n",
               GYRO_LABEL,
               (long int)getRawData.xAxisData,
               (long int)getRawData.yAxisData,
               (long int)getRawData.zAxisData);

    }
    else
    {
        printf("%s Raw Data read FAILED\n\r", GYRO_LABEL);
    }

    returnValue = Gyroscope_readXyzDegreeValue(xdkGyroscope_BMG160_Handle, &getMdegData);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillGyroData(data, &getMdegData);
        printf("%s Converted Data : x = %ld mDeg, y = %ld mDeg, z = %ld mDeg\n\r",
               GYRO_LABEL,
               (long int)getMdegData.xAxisData,
               (long int)getMdegData.yAxisData,
               (long int)getMdegData.zAxisData);

    }
    else
    {
        printf("%s Converted Data read FAILED\n\r", GYRO_LABEL);
    }
}
