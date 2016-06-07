#include "BCDS_LightSensor.h"
#include "XdkSensorHandle.h"
#include "lightdata.h"

static const char LIGHT_SENSOR_LABEL[] = "MAX09 Light Sensor";


static void FillLightData(SensorData* data, uint32_t meas)
{
    data->numMeas = 1;
    snprintf(data->meas[0].name, SENSOR_NAME_SIZE, "%s", "light");

    snprintf(data->meas[0].value,
             SENSOR_VALUE_SIZE,
             "%f",
             (float)meas/1000.0);
}

Retcode_T LightPrivateInit(void* handle)
{
    return LightSensor_init((LightSensor_HandlePtr_T)handle);
}

void LightInit(void)
{
    SensorInit(&LightPrivateInit,
               xdkLightSensor_MAX44009_Handle,
               LIGHT_SENSOR_LABEL);
}

void LightDeinit(void)
{
    LightSensor_deInit(xdkLightSensor_MAX44009_Handle);
}

void LightGetData(SensorData* data)
{
    Retcode_T returnValue = SENSOR_ERROR;
    uint16_t luxRawData = 0;
    uint32_t milliLuxData = 0;

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

    returnValue = LightSensor_readRawData(xdkLightSensor_MAX44009_Handle, &luxRawData);
    if(SENSOR_SUCCESS == returnValue)
    {
        printf("\n%s Raw Data : %x \n", LIGHT_SENSOR_LABEL, luxRawData);
    }
    else
    {
        printf("%s Raw Data read FAILED\n\r", LIGHT_SENSOR_LABEL);
    }

    returnValue = LightSensor_readLuxData(xdkLightSensor_MAX44009_Handle, &milliLuxData);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillLightData(data, milliLuxData);
        printf("%s Data : %d mlux\n\r", LIGHT_SENSOR_LABEL, (unsigned int)milliLuxData);
    }
    else
    {
        printf("%s Data read FAILED\n\r", LIGHT_SENSOR_LABEL);
    }
}
