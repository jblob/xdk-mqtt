#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "controls.h"
#include "leds.h"
#include "jsmn.h"
#include "mqtt.h"
#include "OS_operatingSystem_ih.h"

typedef void(*CommandHandle)(const char* json, jsmntok_t* command, jsmntok_t* val);

void LedControl(const char* json, jsmntok_t* command, jsmntok_t* val);
void PeriodControl(const char* json, jsmntok_t* command, jsmntok_t* val);
void SensorControl(const char* json, jsmntok_t* command, jsmntok_t* val);

#define MAX_TOKENS 20
static jsmn_parser parser;
static jsmntok_t tokens[MAX_TOKENS];
static CommandHandle handlers[] = {&LedControl, &PeriodControl, &SensorControl};
extern OS_taskHandle_tp tickTaskHandle;

bool IsName(const char* expectedName, const char* name, int len)
{
    return (0 == strncmp(expectedName, name, len));
}

extern void CommandConfigHandler(MessageData* data)
{
    const char* commandJson = (char*)data->message->payload;
    const int payloadLen = data->message->payloadlen;
    jsmn_init(&parser);
    int numTokens = jsmn_parse(&parser, commandJson, payloadLen, tokens, MAX_TOKENS);

    int handleSelector = -1;
    int commandToken = -1;
    int valueToken = -1;
    // token 0 is the command JSON object
    // token names are odd, values are even
    for(int tokIdx = 1; tokIdx < numTokens; tokIdx += 2)
    {
        if(IsName("path", commandJson + tokens[tokIdx].start, 4))
        {
            if(IsName("led", commandJson + tokens[tokIdx+1].start, 3))
            {
                handleSelector = 0;
            }
            else if(IsName("per", commandJson + tokens[tokIdx+1].start, 3))
            {
                handleSelector = 1;
            }
            else if(IsName("sensor", commandJson + tokens[tokIdx+1].start, 6))
            {
                handleSelector = 2;
            }
        }
        else if(IsName("command", commandJson + tokens[tokIdx].start, 7))
        {
            commandToken = tokIdx;
        }
        else if(IsName("value", commandJson + tokens[tokIdx].start, 5))
        {
            valueToken = tokIdx;
        }
    }

    if((-1 != handleSelector) && (-1 != commandToken) && (-1 != valueToken))
    {
        handlers[handleSelector](commandJson, &tokens[commandToken + 1], &tokens[valueToken + 1]);
    }
    else
    {
        printf("Unknown command received: %s %d/%d/%d/%d\n\r",
               commandJson,
               numTokens,
               handleSelector,
               commandToken,
               valueToken);
    }
}

void LedControl(const char* json, jsmntok_t* command, jsmntok_t* val)
{
    LED_operations_t op = LED_SET_OFF;

    int value = -1;
    sscanf(json + val->start, "%d", &value);

    if(1 == value)
    {
        op = LED_SET_ON;
    }
    else if(2 == value)
    {
        op = LED_SET_TOGGLE;
    }
    else if(0 != value)
    {
        printf("Unknown led op = %d\n\r", value);
        return;
    }

    int ledidx = NUM_LEDS;

    if(IsName("red", json + command->start, 3))
    {
        ledidx = RED_LED;
    }
    else if(IsName("yellow", json + command->start, 6))
    {
        ledidx = YELLOW_LED;
    }
    else if(IsName("orange", json + command->start, 6))
    {
        ledidx = ORANGE_LED;
    }
    else
    {
        printf("Unknown led color = %s\n\r", json + command->start);
        return;
    }

    LedSet(ledidx, op);
}

void PeriodControl(const char* json, jsmntok_t* command, jsmntok_t* val)
{
    int value = -1;
    sscanf(json + val->start, "%d", &value);

    if(0 < value)
    {
        if(IsName("pub", json + command->start, 3))
        {
            OS_timerChangePeriod(tickTaskHandle, value);
        }
        else if(IsName("sub", json + command->start, 3))
        {
            MqttSetPollingPeriod((uint32_t)value);
        }
        else
        {
            printf("Unknown period = %d\n\r", value);
        }
    }
    else
    {
        printf("Wrong meas period = %d\n\r", value);
    }
}

void SensorControl(const char* json, jsmntok_t* command, jsmntok_t* val)
{
    int value = -1;
    sscanf(json + val->start, "%d", &value);

    if(0 == value || 1 == value)
    {
        if(IsName("env", json + command->start, 3))
        {
            enabledSensors[0] = (bool)value;
        }
        else if(IsName("acc", json + command->start, 3))
        {
            enabledSensors[1] = (bool)value;
        }
        else if(IsName("lgt", json + command->start, 3))
        {
            enabledSensors[2] = (bool)value;
        }
        else if(IsName("gyr", json + command->start, 3))
        {
            enabledSensors[3] = (bool)value;
        }
        else if(IsName("mag", json + command->start, 3))
        {
            enabledSensors[4] = (bool)value;
        }
        else
        {
            printf("Unknown sensor: %s\n\r", json + command->start);
        }
    }
    else
    {
        printf("Wrong sensor setting = %d\n\r", value);
    }
}
