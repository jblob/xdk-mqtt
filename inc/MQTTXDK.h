#pragma once

#include "simplelink.h"
#include "socket.h"

typedef struct Timer Timer;

struct Timer {
    unsigned long systick_period;
    unsigned long end_time;
};

typedef struct Network Network;

struct Network
{
	int my_socket;
	int (*mqttread) (Network*, unsigned char*, int, int);
	int (*mqttwrite) (Network*, unsigned char*, int, int);
	void (*disconnect) (Network*);
};

char expired(Timer*);
void countdown_ms(Timer*, unsigned int);
void countdown(Timer*, unsigned int);
int left_ms(Timer*);

void InitTimer(Timer*);

int xdk_read(Network*, unsigned char*, int, int);
int xdk_write(Network*, unsigned char*, int, int);
void xdk_disconnect(Network*);
void NewNetwork(Network*);

int ConnectNetwork(Network* net, int8_t* addr, int port);
int TLSConnectNetwork(Network*, char*, int, SlSockSecureFiles_t*, unsigned char, unsigned int, char);
