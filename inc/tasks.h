#pragma once

#include <stdio.h>
#include <stdint.h>
#include "OS_operatingSystem_ih.h"

extern OS_taskHandle_tp tickTaskHandle;

void TickInit();
void TickKill();
void CommandHandlerInit();
void WifiConnectInit();
void MqttConnectInit();
void Restart();
