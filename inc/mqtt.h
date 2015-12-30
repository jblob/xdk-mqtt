#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"
#include "MQTTClient.h"

typedef enum returnCode MqttErrorCode;

extern int  MqttInit();
extern void MqttDeinit();
extern int  MqttReconnect();
extern int  MqttSendData(NameValue* data);
extern void MqttSubscribe(messageHandler callback);
extern void MqttYield(void* context);
extern void MqttStopPolling();
extern void MqttSetPollingPeriod(uint32_t period);
