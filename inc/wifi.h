#pragma once

typedef void (*WifiCallback)(int status);

int WiFiInit(WifiCallback callback);
int WiFiDeinit(void);
void WiFiPrintIP(void);
