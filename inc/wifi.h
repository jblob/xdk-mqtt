#pragma once

typedef void (*WifiCallback)(int status);

int WiFiInit();
int WiFiInit(WifiCallback callback);
int WiFiDeinit();
void WiFiPrintIP();
