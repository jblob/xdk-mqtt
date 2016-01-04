#include "magnetometer.h"
#include "xdk_sensors.h"
#include "magnetodata.h"

static const char MAGNETO_LABEL[] = "BMM150 Magnetometer";

static void FillMagnetoData(SensorData* data, magnetometer_xyzData_t* meas)
{
    data->numMeas = 1;
    snprintf(data->meas[0].name, SENSOR_NAME_SZ, "%s", "magnetometer");

    snprintf(data->meas[0].value,
             SENSOR_VALUE_SZ,
             "{\"x\":%f, \"y\":%f, \"z\":%f}",
             (float)meas->xAxisData,
             (float)meas->yAxisData,
             (float)meas->zAxisData);
}

void MagnetoInit()
{
    SensorInit(&magnetometerInit,
               xdkMagnetometer_BMM150_Handle,
               MAGNETO_LABEL);
}

void MagnetoDeinit()
{
}

void MagnetoGetData(SensorData* data)
{
    sensor_errorType_t returnValue = SENSOR_ERROR;
    magnetometer_xyzData_t getMagData = {0};

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

    returnValue = magnetometerReadXyzLsbData(xdkMagnetometer_BMM150_Handle, &getMagData);
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

    returnValue = magnetometerReadXyzTeslaData(xdkMagnetometer_BMM150_Handle, &getMagData);
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
