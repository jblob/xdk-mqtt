#include "tasks.h"
#include "em_wdog.h"
#include "sensordata.h"
#include "leds.h"
#include "mqtt.h"
#include "wifi.h"
#include "controls.h"

static void Tick(void* context);
static void MqttConnectionTask(void* context);
static void MqttSubscriptionTask(void* context);
static void WifiConnectionCallback(int status);
static void WifiReconnect(void* context);

OS_taskHandle_tp tickTaskHandle = NULL;
static OS_taskHandle_tp commandTaskHandle;
static OS_taskHandle_tp reconnectTaskHandle;
static OS_taskHandle_tp subscriptionTaskHandle;
static OS_taskHandle_tp wifiReconnectHandle;

static uint32_t PUBLISH_PERIOD = 3000;
static const uint32_t COMMAND_PERIOD = 1000;
static const uint32_t RECONNECT_PERIOD = 4000;
static const uint32_t PUBLISH_BLOCK_TIME = 0xffff;
static const uint32_t TASK_STACK_SIZE = 1000;
static const uint32_t TASK_PRIO = 4;

static bool killTick = false;
static bool wifiConnected = false;

void TickInit(void)
{
    WDOG_Feed();
    int8_t retValPerSwTimer = OS_ERR_NOT_ENOUGH_MEMORY;
    retValPerSwTimer = OS_taskCreate(Tick,
                                     (const int8_t *) "PublishData",
                                     TASK_STACK_SIZE,
                                     TASK_PRIO - 2,
                                     tickTaskHandle);

    if(OS_ERR_NO_ERROR != retValPerSwTimer)
    {
        printf("Not enough memory to start Tick Timer!");
        return;
    }
    printf("Tick Init Success!\n\r");
}

void TickKill(void)
{
    killTick = true;
}

static void Tick(void* context)
{
    for(;;)
    {
        WDOG_Feed();
        if(killTick)
        {
            killTick = false;
            OS_taskDelete(NULL);
        }

        context = context;
        SensorData data;

        for(uint32_t sensor = 0; sensor < NUM_SENSORS; ++sensor)
        {
            if(enabledSensors[sensor])
            {
                // Get data
                sensors[sensor](&data);

                for(uint32_t meas = 0; meas < data.numMeas; ++meas)
                {
                    WDOG_Feed();
                    if(0 > MqttSendData(&data.meas[meas]))
                    {
                        // TODO: Check why taskDelete/taskSuspend deletes/suspens __current__ thread
                        // OS_taskDelete(commandTaskHandle);
                        MqttStopPolling();
                        printf("Sending data FAILED! Restarting WiFi and MQTT!\n");
                        Restart();
                    }
                    else
                    {
                        printf("Data sent successfully!\n");
                        LedSet(ORANGE_LED, LED_SET_TOGGLE);
                    }
                }
            }
        }
        OS_taskDelay(PUBLISH_PERIOD);
    }
}

void Restart(void)
{
    WDOG_Feed();
    OS_taskDelete(commandTaskHandle);
    OS_taskDelete(reconnectTaskHandle);
    OS_taskDelete(subscriptionTaskHandle);
    OS_taskDelete(wifiReconnectHandle);
    MqttDeinit();
    printf("WiFi deinit = %d\n\r", WiFiDeinit());
    wifiConnected = false;
    MqttConnectInit();
    OS_taskCreate(WifiReconnect,
                  (const int8_t *) "WiFi Reconnection",
                  TASK_STACK_SIZE,
                  TASK_PRIO - 1,
                  wifiReconnectHandle);
}

void CommandHandlerInit(void)
{
    WDOG_Feed();
    int8_t retValPerSwTimer = OS_ERR_NOT_ENOUGH_MEMORY;
    retValPerSwTimer = OS_taskCreate(MqttYield,
                                     (const int8_t *) "MQTT Commands",
                                     TASK_STACK_SIZE,
                                     TASK_PRIO,
                                     commandTaskHandle);

    if(OS_ERR_NO_ERROR != retValPerSwTimer)
    {
        printf("Error occurred in creating MQTT command task \n\r");
    }
    else
    {
        printf("MQTT command task Success \n\r");
    }
}

void WifiConnectInit(void)
{
    WDOG_Feed();
    printf("Connecting WiFi...\n");
    if(-1 == WiFiInit(&WifiConnectionCallback))
    {
        printf("Connection to WLAN failed. Retrying in %d ms\n", (int)RECONNECT_PERIOD);
        OS_taskCreate(WifiReconnect,
                      (const int8_t *) "WiFi Reconnection",
                      TASK_STACK_SIZE,
                      TASK_PRIO - 1,
                      wifiReconnectHandle);
    }
}

void WifiReconnect(void* context)
{
    for(;;)
    {
        WDOG_Feed();
        if(wifiConnected)
        {
            OS_taskDelete(NULL);
        }
        context = context;
        OS_taskDelay(RECONNECT_PERIOD);
        WDOG_Feed();
        if(0 == WiFiInit(&WifiConnectionCallback))
        {
            OS_taskDelete(NULL);
        }
    }
}

void MqttConnectInit(void)
{
    WDOG_Feed();
    OS_taskCreate(MqttConnectionTask,
                  (const int8_t *) "MQTT Connection",
                  TASK_STACK_SIZE,
                  TASK_PRIO - 2,
                  reconnectTaskHandle);
}

void MqttSubscriptionInit(void)
{
    WDOG_Feed();
    OS_taskCreate(MqttSubscriptionTask,
                  (const int8_t *) "MQTT Subscription",
                  TASK_STACK_SIZE,
                  TASK_PRIO - 2,
                  subscriptionTaskHandle);
}

static void WifiConnectionCallback(int status)
{
    WDOG_Feed();
    if(0 == status)
    {
        printf("WiFi connected OK!\n");
        WiFiPrintIP();
        wifiConnected = true;
    }
    else
    {
        Restart();
    }

}

static void MqttConnectionTask(void* context)
{
    for(;;)
    {
        WDOG_Feed();
        context = context;
        if(wifiConnected)
        {
            printf("Connecting MQTT...\n");
            if(0 == MqttInit())
            {
                MqttSubscriptionInit();
                OS_taskDelete(NULL);
            }
            else
            {
                printf("Connection to MQTT server failed. Retrying in %d ms\n", (int)RECONNECT_PERIOD);
                Restart();
            }
        }
        OS_taskDelay(RECONNECT_PERIOD);
    }
}

static void MqttSubscriptionTask(void* context)
{
    for(;;)
    {
        WDOG_Feed();
        context = context;
        if(wifiConnected)
        {
            printf("Subscribing to MQTT...\n");
            if(0 == MqttSubscribe(&CommandConfigHandler))
            {
                TickInit();
                CommandHandlerInit();
                OS_taskDelete(NULL);
            }
            else
            {
                printf("Subscription to MQTT server failed. Retrying in %d ms\n", (int)RECONNECT_PERIOD);
                Restart();
            }
        }
        OS_taskDelay(RECONNECT_PERIOD);
    }
}
