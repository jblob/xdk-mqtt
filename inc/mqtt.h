#pragma once

#include <stdio.h>
#include <stdint.h>
#include "sensordata.h"
#include "MQTTClient.h"

typedef enum returnCode MqttErrorCode;

int  MqttInit();
void MqttDeinit();
int  MqttReconnect();
int  MqttSendData(NameValue* data);
void MqttSubscribe(messageHandler callback);
void MqttYield(void* context);
void MqttStopPolling();
void MqttSetPollingPeriod(uint32_t period);
