#pragma once

#include <stdio.h>
#include <stdint.h>
#include "OS_operatingSystem_ih.h"

extern OS_taskHandle_tp tickTaskHandle;

void TickInit(void);
void TickKill(void);
void CommandHandlerInit(void);
void WifiConnectInit(void);
void MqttConnectInit(void);
void Restart(void);
