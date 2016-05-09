#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"
#include "MQTTClient.h"

typedef enum returnCode MqttErrorCode;

int  MqttInit(void);
void MqttDeinit(void);
int  MqttReconnect(void);
int  MqttSendData(NameValue* data);
int  MqttSubscribe(messageHandler callback);
void MqttYield(void* context);
void MqttStopPolling(void);
void MqttSetPollingPeriod(uint32_t period);
