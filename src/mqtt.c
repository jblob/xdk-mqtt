#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "BCDS_PowerMgt.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "mqtt.h"
#include "MQTTClient.h"

// auto generated file with credentials
#include "credentials.h"

static MqttErrorCode MqttConnect(void);
static size_t SerializeData(NameValue* data, char* msg);

static Network mqttNet;
static Client mqttClient;

#define MQTT_BUFF_SIZE 1000
static uint8_t buf[MQTT_BUFF_SIZE];
static uint8_t readbuf[MQTT_BUFF_SIZE];

static uint8_t MESSAGE_ID = 0;
static bool deletePolling = false;
static uint32_t pollingPeriod = 1000;
static int32_t MQTT_YIELD_TIMEOUT = 50;

int MqttInit(void)
{
    printf("Mqtt Init\n");
    MqttErrorCode returnValue = FAILURE;
    printf ("<mqtt.c><MqttInit>calling NewNetwork\n\r");
    NewNetwork(&mqttNet);
    printf ("<mqtt.c><MqttInit>returned from NewNetwork\n\r");
    returnValue = MqttConnect();
    printf ("<mqtt.c><MqttInit>MqttConnect returned %d\n\r", returnValue);
    if(SUCCESS == returnValue)
    {
        printf("Mqtt Success\n");
        deletePolling = false;
    }
    else
    {
        printf("Mqtt initialization Failed\n");
    }

    return returnValue;
}

int MqttReconnect(void)
{
    return MqttConnect();
}

void MqttDeinit(void)
{
    printf("Mqtt disconnect!\n");
    mqttNet.disconnect(&mqttNet);
}

int MqttSendData(NameValue* data)
{
    MQTTMessage msg;
    msg.id = MESSAGE_ID++;
    msg.qos = 0;
    char msgBuff[MQTT_BUFF_SIZE];
    msg.payload = msgBuff;
    msg.payloadlen = SerializeData(data, msgBuff);
    int ret = MQTTPublish(&mqttClient, MQTT_TOPIC"data", &msg);
    printf("Mqtt Sent :: %d %d\n", ret, msg.payloadlen);
    return ret;
}

int MqttSubscribe(messageHandler callback)
{
    int ret = MQTTSubscribe(&mqttClient, "/v1/"MQTT_USER"/cmd", QOS0, callback);
    printf("MqttSubscribe:: %s %d\n", "/v1/"MQTT_USER"/cmd", ret);
    return ret;
}

void MqttYield(void* context)
{
    for (;;)
    {
        if(deletePolling)
        {
            // Note: This kills the thread.
            vTaskDelete(NULL);
        }
        context = context;
        printf("Polling MQTT command queue\n");
        MQTTYield(&mqttClient, MQTT_YIELD_TIMEOUT);
        //OS_taskDelay(pollingPeriod);
    	vTaskDelay(pollingPeriod/portTICK_PERIOD_MS);
    }
}

MqttErrorCode MqttConnect(void)
{
    MqttErrorCode ret = FAILURE;
    ret = ConnectNetwork(&mqttNet, (int8_t*)MQTT_SERVER, MQTT_SERVER_PORT);
    printf ("<mqtt.c><MqttConnect>ConnectNetwork returned %d\n\r", ret);
    if(SUCCESS == ret)
    {
        MQTTClient(&mqttClient, &mqttNet, 1000, buf, MQTT_BUFF_SIZE, readbuf, MQTT_BUFF_SIZE);

        MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
        data.willFlag = 0;
        data.MQTTVersion = 3;
        data.clientID.cstring = MQTT_CLIENTID;
        data.username.cstring = MQTT_USER;
        data.password.cstring = MQTT_PASSWORD;

        data.keepAliveInterval = 10;
        data.cleansession = 1;
        printf("Connecting to %s %d with user: %s, ps: %s\n",
                MQTT_SERVER,
                MQTT_SERVER_PORT,
                MQTT_USER,
                MQTT_PASSWORD);

        ret = MQTTConnect(&mqttClient, &data);
        if(SUCCESS == ret)
        {
            printf("Connected %d\n", ret);
        }
        else
        {
            printf("Mqtt Connection failed!\n");
        }
    }
    return ret;
}

static size_t SerializeData(NameValue* data, char* msg)
{
    return sprintf(msg,
                   "{\"meaning\":\"%s\",\"path\":\"%s\",\"value\":%s}",
                   data->name,
                   data->name,
                   data->value);
}

void MqttStopPolling(void)
{
    deletePolling = true;
}

void MqttSetPollingPeriod(uint32_t period)
{
    pollingPeriod = period;
}

