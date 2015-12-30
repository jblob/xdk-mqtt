#pragma once

#include <stdio.h>
#include <stdint.h>
#include "OS_operatingSystem_ih.h"

extern OS_taskHandle_tp tickTaskHandle;

extern void TickInit();
extern void TickKill();
extern void CommandHandlerInit();
extern void WifiConnectInit();
extern void MqttConnectInit();
extern void Restart();
