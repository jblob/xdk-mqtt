/*
 * MQTTXDK.c
 *
 *  Created on: 16 wrz 2015
 *      Author: root
 */

#include "MQTTXDK.h"
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#define CMN_TIMER_AUTO_RELOAD_ON         1      /**< Turn on Auto Reload of the timers */
#define TIMER_NOT_ENOUGH_MEMORY        (-1L)	/**<Macro to define not enough memory error in timer*/

static const int32_t TIMER_TICK_PERIOD = 10;
static unsigned long MilliTimer = 0;
static xTimerHandle sysTickIntHandle = NULL;

void sysTickIntHandler(void *pvParameters)
{
    pvParameters = pvParameters;
    MilliTimer += TIMER_TICK_PERIOD;
}

char expired(Timer* timer)
{
    long left = timer->end_time - MilliTimer;
    return (left < 0);
}

void countdown_ms(Timer* timer, unsigned int timeout)
{
    timer->end_time = MilliTimer + timeout;
}

void countdown(Timer* timer, unsigned int timeout)
{
    timer->end_time = MilliTimer + (timeout * 1000);
}

int left_ms(Timer* timer)
{
    long left = timer->end_time - MilliTimer;
    return (left < 0) ? 0 : left;
}

void InitTimer(Timer* timer)
{
    timer->end_time = 0;
}

int xdk_read(Network* net, uint8_t* buffer, int len, int timeout)
{
    SlTimeval_t timeVal;
    SlFdSet_t fdset;
    int rc = 0;
    int recvLen = 0;

    timeVal.tv_sec = 0;
    timeVal.tv_usec = timeout * 1000;

    SL_FD_ZERO(&fdset);
    SL_FD_SET(net->my_socket, &fdset);

    if (sl_Select(net->my_socket + 1, &fdset, NULL, NULL, &timeVal) == 1)
    {
        do
        {
            rc = sl_Recv(net->my_socket, buffer + recvLen, len - recvLen, 0);
            recvLen += rc;
        } while(recvLen < len);
    }
    return recvLen;
}

int xdk_write(Network* net, uint8_t* buffer, int len, int timeout)
{
    SlTimeval_t timeVal;
    SlFdSet_t fdset;
    int rc = 0;
    int readySock;

    SL_FD_ZERO(&fdset);
    SL_FD_SET(net->my_socket, &fdset);

    timeVal.tv_sec = 0;
    timeVal.tv_usec = timeout * 1000;
    do
    {
        readySock = sl_Select(net->my_socket + 1, NULL, &fdset, NULL, &timeVal);
    } while(readySock != 1);
    rc = sl_Send(net->my_socket, buffer, len, 0);
    return rc;
}
void xdk_disconnect(Network* net)
{
    sl_Close(net->my_socket);
}

void NewNetwork(Network* net)
{
    net->my_socket = 0;
    net->mqttread = xdk_read;
    net->mqttwrite = xdk_write;
    net->disconnect = xdk_disconnect;
}

int ConnectNetwork(Network* net, int8_t* addr, int port)
{
    SlSockAddrIn_t sAddr;
    int addrSize;
    int retVal = -1;
    int timerRetVal = TIMER_NOT_ENOUGH_MEMORY;
    unsigned long ipAddress;

    printf ("<MQTTXDK.c><ConnectNetwork> (1) retval = %d\n\r", retVal);
    sl_NetAppDnsGetHostByName(addr, strlen((char*)addr), &ipAddress, AF_INET);

    sAddr.sin_family = AF_INET;
    sAddr.sin_port = sl_Htons((unsigned short)port);
    sAddr.sin_addr.s_addr = sl_Htonl(ipAddress);

    addrSize = sizeof(SlSockAddrIn_t);

    net->my_socket = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
    if(net->my_socket < 0)
    {
        printf("Failed to open socket!\n\r");
        return -1;
    }

    retVal = sl_Connect(net->my_socket, ( SlSockAddr_t *)&sAddr, addrSize);
    printf ("<MQTTXDK.c><ConnectNetwork> (2) retval = %d\n\r", retVal);
    if(retVal < 0)
    {
        printf("Failed to connect to socket!\n\r");
        sl_Close(net->my_socket);
        return retVal;
    }
    sysTickIntHandle = xTimerCreate((const char * const) "MqttTimer",
                                      TIMER_TICK_PERIOD/portTICK_RATE_MS,
 									  CMN_TIMER_AUTO_RELOAD_ON,
                                      NULL,
                                      sysTickIntHandler);

    if(NULL == sysTickIntHandle)
    {
        printf("Not enough memory to create MQTT Timer!");
    }
    timerRetVal = xTimerStart(sysTickIntHandle, 0xFFFF/portTICK_RATE_MS);
    if (TIMER_NOT_ENOUGH_MEMORY == timerRetVal)
    {
    	printf ("Timer Out Of Memory - should think about to stop here\n\r");
    	// return -1; // ??
    }

//    retVal = xTimerStart(sysTickIntHandle, 0xFFFF);
    printf ("<MQTTXDK.c><ConnectNetwork> portTICK_RATE_MS = %d\n\r", portTICK_RATE_MS);
    printf ("<MQTTXDK.c><ConnectNetwork> (3) retval = %d\n\r", retVal);
    if(pdFAIL == retVal)
    {
        printf("Not enough memory to start MQTT Timer!");
    }

    printf ("<MQTTXDK.c><ConnectNetwork> returns %d\n\r", retVal);

    printf ("<MQTTXDK.c><ConnectNetwork> (4) retval = %d\n\r", retVal);
    return retVal;
}
