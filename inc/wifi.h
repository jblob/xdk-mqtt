#pragma once

typedef void (*WifiCallback)(int status);

extern int WiFiInit();
extern int WiFiInit(WifiCallback callback);
extern int WiFiDeinit();
extern void WiFiPrintIP();
