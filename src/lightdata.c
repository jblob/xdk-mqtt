#include "lightsensor.h"
#include "xdk_sensors.h"
#include "lightdata.h"

static const char LIGHT_SENSOR_LABEL[] = "MAX09 Light Sensor";

void InitLightData(SensorData* storage)
{
    storage->numMeas = 1;
    sprintf(storage->meas[0].name, "%s", "light");
}

void FillLightData(SensorData* data, lightsensor_data_t* meas)
{
    InitLightData(data);
    sprintf(data->meas[0].value,
            "%f",
            (float)meas->sensorDataInMilliLux/1000.0);
}

extern void LightInit()
{
    SensorInit(&lightsensorInit,
               xdkLightsensors_MAX09_Handle,
               LIGHT_SENSOR_LABEL);
}

extern void LightDeinit()
{
}

extern void LightGetData(SensorData* data)
{
    sensor_errorType_t returnValue = SENSOR_ERROR;
    lightsensor_data_t luxRawData = {0};
    lightsensor_data_t milliLuxData = {0};

    returnValue = lightsensorReadRawData(xdkLightsensors_MAX09_Handle, &luxRawData);
    if(SENSOR_SUCCESS == returnValue)
    {
        printf("\n%s Raw Data : %x \n", LIGHT_SENSOR_LABEL, luxRawData.sensorRawData);
    }
    else
    {
        printf("%s Raw Data read FAILED\n\r", LIGHT_SENSOR_LABEL);
    }

    returnValue = lightsensorReadLuxData(xdkLightsensors_MAX09_Handle, &milliLuxData);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillLightData(data, &milliLuxData);
        printf("%s Data : %d mlux\n\r", LIGHT_SENSOR_LABEL, (unsigned int)milliLuxData.sensorDataInMilliLux);
    }
    else
    {
        printf("%s Data read FAILED\n\r", LIGHT_SENSOR_LABEL);
    }
}
