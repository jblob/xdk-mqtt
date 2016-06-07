#include "BCDS_Environmental.h"
#include "XdkSensorHandle.h"
#include "envdata.h"

static const char ENV_SENSOR_LABEL[] = "BME280 Environmental Sensor";

static void FillEnvData(SensorData* data, Environmental_Data_T* meas)
{
    data->numMeas = 3;

    snprintf(data->meas[0].name,  SENSOR_NAME_SIZE,  "%s", "temperature");
    snprintf(data->meas[0].value, SENSOR_VALUE_SIZE, "%f", meas->temperature/1000.0);

    snprintf(data->meas[1].name,  SENSOR_NAME_SIZE,  "%s", "pressure");
    snprintf(data->meas[1].value, SENSOR_VALUE_SIZE, "%f", meas->pressure/100.0);

    snprintf(data->meas[2].name,  SENSOR_NAME_SIZE,  "%s", "humidity");
    snprintf(data->meas[2].value, SENSOR_VALUE_SIZE, "%d", (int)meas->humidity);
}

static Retcode_T EnvPrivateInit(void* handle)
{
    return Environmental_init((Environmental_HandlePtr_T)handle);
}

void EnvSensorInit()
{
    SensorInit(&EnvPrivateInit,
               xdkEnvironmental_BME280_Handle,
               ENV_SENSOR_LABEL);
}

void EnvSensorDeinit()
{
    Environmental_deInit(xdkEnvironmental_BME280_Handle);
}

void EnvSensorGetData(SensorData* data)
{
    Retcode_T returnValue = SENSOR_ERROR;

    Environmental_Data_T bme280 = {0};
    Environmental_LsbData_T bme280lsb = {0};

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

    returnValue = Environmental_readDataLSB(xdkEnvironmental_BME280_Handle, &bme280lsb);
    if(SENSOR_SUCCESS == returnValue)
    {
        printf("\n%s Raw Data : p = %ld, t = %ld, h = %ld\n",
               ENV_SENSOR_LABEL,
               (long int)bme280lsb.pressure,
               (long int)bme280lsb.temperature,
               (long int)bme280lsb.humidity);

    }
    else
    {
        printf("%s Raw Data read FAILED\n\r", ENV_SENSOR_LABEL);
    }

    returnValue = Environmental_readData(xdkEnvironmental_BME280_Handle, &bme280);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillEnvData(data, &bme280);
        printf("%s Conversion Data : p = %ld Pa, t = %ld mDeg, h = %ld %%\n\r",
               ENV_SENSOR_LABEL,
               (long int)bme280.pressure,
               (long int)bme280.temperature,
               (long int)bme280.humidity);
    }
    else
    {
        printf("%s Conversion Data read FAILED\n\r", ENV_SENSOR_LABEL);
    }
}
