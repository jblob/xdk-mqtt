#include "magnetometer.h"
#include "xdk_sensors.h"
#include "magnetodata.h"

static const char MAGNETO_LABEL[] = "BMM150 Magnetometer";

void InitMagnetoData(SensorData* storage)
{
    storage->numMeas = 1;
    sprintf(storage->meas[0].name, "%s", "magnetometer");
}

void FillMagnetoData(SensorData* data, magnetometer_xyzData_t* meas)
{
    InitMagnetoData(data);
    sprintf(data->meas[0].value,
            "{\"x\":%f, \"y\":%f, \"z\":%f}",
            (float)meas->xAxisData,
            (float)meas->yAxisData,
            (float)meas->zAxisData);
}

extern void MagnetoInit()
{
    SensorInit(&magnetometerInit,
               xdkMagnetometer_BMM150_Handle,
               MAGNETO_LABEL);
}

extern void MagnetoDeinit()
{
}

extern void MagnetoGetData(SensorData* data)
{
    sensor_errorType_t returnValue = SENSOR_ERROR;
    magnetometer_xyzData_t getMagData = {0};

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
