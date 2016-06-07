#include "BCDS_Magnetometer.h"
#include "XdkSensorHandle.h"
#include "magnetodata.h"

static const char MAGNETO_LABEL[] = "BMM150 Magnetometer";

static void FillMagnetoData(SensorData* data, Magnetometer_XyzData_T* meas)
{
    data->numMeas = 1;
    snprintf(data->meas[0].name, SENSOR_NAME_SIZE, "%s", "magnetometer");

    snprintf(data->meas[0].value,
             SENSOR_VALUE_SIZE,
             "{\"x\":%f, \"y\":%f, \"z\":%f}",
             (float)meas->xAxisData,
             (float)meas->yAxisData,
             (float)meas->zAxisData);
}

Retcode_T PrivateInit(void* handle)
{
    return Magnetometer_init((Magnetometer_HandlePtr_T)handle);
}

void MagnetoInit(void)
{
    SensorInit(&PrivateInit,
               xdkMagnetometer_BMM150_Handle,
               MAGNETO_LABEL);
}

void MagnetoDeinit(void)
{
    Magnetometer_deInit(xdkMagnetometer_BMM150_Handle);
}

void MagnetoGetData(SensorData* data)
{
    Retcode_T returnValue = SENSOR_ERROR;
    Magnetometer_XyzData_T getMagData = {0};

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

    returnValue = Magnetometer_readXyzLsbData(xdkMagnetometer_BMM150_Handle, &getMagData);
    if(SENSOR_SUCCESS == returnValue)
    {
        printf("\n%s Raw Data : x = %ld, y = %ld, z = %ld\n",
               MAGNETO_LABEL,
               (long int)getMagData.xAxisData,
               (long int)getMagData.yAxisData,
               (long int)getMagData.zAxisData);
    }
    else
    {
        printf("%s Raw Data read FAILED\n\r", MAGNETO_LABEL);
    }

    returnValue = Magnetometer_readXyzTeslaData(xdkMagnetometer_BMM150_Handle, &getMagData);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillMagnetoData(data, &getMagData);
        printf("\n%s Converted Data : x = %ld uT, y = %ld uT, z = %ld uT\n\r",
               MAGNETO_LABEL,
               (long int)getMagData.xAxisData,
               (long int)getMagData.yAxisData,
               (long int)getMagData.zAxisData);
    }
    else
    {
        printf("%s Converted Data read FAILED\n\r", MAGNETO_LABEL);
    }
}
