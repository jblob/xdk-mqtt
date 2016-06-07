#include <stdio.h>
#include <stdint.h>

#include "XdkSystemStartup.h"
#include "mqtt.h"

// sensors
#include "sensordata.h"
#include "envdata.h"
#include "acceldata.h"
#include "lightdata.h"
#include "gyrodata.h"
#include "magnetodata.h"

// peripherals
#include "leds.h"
#include "wifi.h"
#include "xdkbutton.h"

#include "tasks.h"
#include "controls.h"
#include "em_wdog.h"

SensorGetter sensors[] = {&EnvSensorGetData,
                          &AccelGetData,
                          &LightGetData,
                          &GyroGetData,
                          &MagnetoGetData};

bool enabledSensors[] = {true, // Env Sensor
                         true, // Accel Sensor
                         true, // Light Sensor
                         true, // Gyro Sensor
                         true  // Magneto Sensor
                        };

void ButtonOneCallback(void *handle, uint32_t userParameter);
void WDogInit(void);

int main(void)
{
    systemStartup();
}

void appInitSystem(xTimerHandle xTimer)
{
    (void) (xTimer);
    LedInit();
    ButtonOneInit(&ButtonOneCallback);
    MagnetoInit();
    GyroInit();
    LightInit();
    AccelInit();
    EnvSensorInit();
    WDogInit();
    WifiConnectInit();
    MqttConnectInit();
}

void ButtonOneCallback(void *handle, uint32_t userParameter)
{
    printf("Button %d pressed\n\r", (int)userParameter);

    if(IsButtonPressed(handle))
    {
        printf("Button pressed!\n");
        LedSet(RED_LED, LED_SET_ON);
    }

    if(IsButtonReleased(handle))
    {
        printf("Button released!\n");
        LedSet(RED_LED, LED_SET_OFF);
        MqttStopPolling();
        TickKill();
        Restart();
    }
}

void WDogInit(void)
{
    WDOG_Init_TypeDef wdConfig = { false,              /* Do not Start watchdog when init done */
                                   false,              /* WDOG not counting during debug halt */
                                   false,              /* WDOG not counting when in EM2 */
                                   false,              /* WDOG not counting when in EM3 */
                                   false,              /* EM4 can be entered */
                                   false,              /* Do not block disabling LFRCO/LFXO in CMU */
                                   false,              /* Do not lock WDOG configuration (if locked, reset needed to unlock) */
                                   wdogClkSelLFXO,     /* Select 32768HZ LFXO as clock source for WDOG oscillator */
                                   wdogPeriod_256k     /* Set longest possible timeout period */
                                };
    printf("Initializing Watch Dog\n");
    WDOG_Init(&wdConfig);
    printf("Turning on Watch Dog\n");
    WDOG_Enable(true);
    printf("Watch Dog ON\n");
}
