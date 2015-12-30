#include "environmentalSensor.h"
#include "xdk_sensors.h"
#include "envdata.h"

static const char ENV_SENSOR_LABEL[] = "BME280 Environmental Sensor";

extern void InitEnvData(SensorData* storage)
{
    storage->numMeas = 3;
    sprintf(storage->meas[0].name, "%s", "temperature");
    sprintf(storage->meas[1].name, "%s", "pressure");
    sprintf(storage->meas[2].name, "%s", "humidity");
}

void FillEnvData(SensorData* data, environmental_data_t* meas)
{
    InitEnvData(data);
    sprintf(data->meas[0].value, "%f", meas->temperature/1000.0);
    sprintf(data->meas[1].value, "%f", meas->pressure/100.0);
    sprintf(data->meas[2].value, "%d", (int)meas->humidity);
}

extern void EnvSensorInit()
{
    SensorInit(&environmentalInit,
               xdkEnvironmental_BME280_Handle,
               ENV_SENSOR_LABEL);
}

extern void EnvSensorDeinit()
{
}

extern void EnvSensorGetData(SensorData* data)
{
    sensor_errorType_t returnValue = SENSOR_ERROR;
    uint8_t chipid = 0;
    environmentalChipID(xdkEnvironmental_BME280_Handle, &chipid);

    environmental_data_t bme280 = {0};
    environmental_lsbData_t bme280lsb = {0};

    returnValue = environmentalReadDataLSB(xdkEnvironmental_BME280_Handle, &bme280lsb);
    if(SENSOR_SUCCESS == returnValue)
    {
        printf("\n%s Raw Data : id = %d, p = %ld, t = %ld, h = %ld\n",
               ENV_SENSOR_LABEL,
               chipid,
               (long int)bme280lsb.pressure,
               (long int)bme280lsb.temperature,
               (long int)bme280lsb.humidity);

    }
    else
    {
        printf("%s Raw Data read FAILED\n\r", ENV_SENSOR_LABEL);
    }

    returnValue = environmentalReadData(xdkEnvironmental_BME280_Handle, &bme280);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillEnvData(data, &bme280);
        printf("%s Conversion Data : id = %d, p = %ld Pa, t = %ld mDeg, h = %ld %%\n\r",
               ENV_SENSOR_LABEL,
               chipid,
               (long int)bme280.pressure,
               (long int)bme280.temperature,
               (long int)bme280.humidity);
    }
    else
    {
        printf("%s Conversion Data read FAILED\n\r", ENV_SENSOR_LABEL);
    }
}
