#include "environmentalSensor.h"
#include "xdk_sensors.h"
#include "envdata.h"

static const char ENV_SENSOR_LABEL[] = "BME280 Environmental Sensor";

static void FillEnvData(SensorData* data, environmental_data_t* meas)
{
    data->numMeas = 3;

    snprintf(data->meas[0].name,  SENSOR_NAME_SIZE,  "%s", "temperature");
    snprintf(data->meas[0].value, SENSOR_VALUE_SIZE, "%f", meas->temperature/1000.0);

    snprintf(data->meas[1].name,  SENSOR_NAME_SIZE,  "%s", "pressure");
    snprintf(data->meas[1].value, SENSOR_VALUE_SIZE, "%f", meas->pressure/100.0);

    snprintf(data->meas[2].name,  SENSOR_NAME_SIZE,  "%s", "humidity");
    snprintf(data->meas[2].value, SENSOR_VALUE_SIZE, "%d", (int)meas->humidity);
}

void EnvSensorInit()
{
    SensorInit(&environmentalInit,
               xdkEnvironmental_BME280_Handle,
               ENV_SENSOR_LABEL);
}

void EnvSensorDeinit()
{
}

void EnvSensorGetData(SensorData* data)
{
    sensor_errorType_t returnValue = SENSOR_ERROR;
    uint8_t chipid = 0;
    environmentalChipID(xdkEnvironmental_BME280_Handle, &chipid);

    environmental_data_t bme280 = {0};
    environmental_lsbData_t bme280lsb = {0};

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

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
