#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>

#include "PIp.h"
#include "Serval_Ip.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include "em_wdog.h"

// auto generated file with credentials
#include "credentials.h"

void WifiConnectedCallback(WLI_connectStatus_t connectStatus);

static int wifiInitDone = 0;
static WifiCallback externCallback = NULL;

int WiFiInit(WifiCallback callback)
{
    externCallback = callback;
    WDOG_Feed();
    printf("WiFi Init\r\n");
    int retVal = -1;
    NCI_ipSettings_t myIpSettings;
    memset(&myIpSettings, (uint32_t) 0, sizeof(myIpSettings));
    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    int8_t ipAddressMy[15] = {0};

    WLI_connectSSID_t connectSSID;
    WLI_connectPassPhrase_t connectPassPhrase;

    if(0 == wifiInitDone && WLI_SUCCESS != WLI_init())
    {
        printf("Error occurred initializing WLAN \r\n ");
        return retVal;
    }

    wifiInitDone = 1;

    printf("Connecting to: %s\r\n ", WLAN_CONNECT_WPA_SSID);

    connectSSID = (WLI_connectSSID_t) WLAN_CONNECT_WPA_SSID;
    connectPassPhrase = (WLI_connectPassPhrase_t) WLAN_CONNECT_WPA_PASS;

    if (WLI_SUCCESS == WLI_connectWPA(connectSSID, connectPassPhrase, NULL))
    {
        WDOG_Feed();
        NCI_getIpSettings(&myIpSettings);
        *IpaddressHex = Basics_htonl(myIpSettings.ipV4);
        (void)Ip_convertAddrToString(IpaddressHex,(char *)&ipAddressMy);
        printf("Connected to WPA network successfully. \r\n ");
        printf(" Ip address of the device: %s \r\n ",ipAddressMy);
        externCallback(0);
        retVal = 0;
    }
    else
    {
        printf("Error occurred connecting %s \r\n ",WLAN_CONNECT_WPA_SSID);
        return retVal;
    }

    return retVal;
}

int WiFiDeinit(void)
{
    printf("WiFi disconnect!\n");
    return WLI_disconnect(NULL);
}

void WiFiPrintIP(void)
{
    NCI_ipSettings_t myIpSettings;
    memset(&myIpSettings, 0, sizeof(myIpSettings));

    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    NCI_getIpSettings(&myIpSettings);
    *IpaddressHex = Basics_htonl(myIpSettings.ipV4);

    int8_t ipAddressMy[15] = {0};
    (void)Ip_convertAddrToString(IpaddressHex,(char *)&ipAddressMy);

    printf("IP address of the device: %s \r\n ", ipAddressMy);
}

void WifiConnectedCallback(WLI_connectStatus_t connectStatus)
{
    int ret = 0;
    WDOG_Feed();
    printf("WiFi callback with status = %d\r\n", (int)connectStatus);
    if(WLI_CONNECTED == connectStatus)
    {
        printf("Connected to WPA network successfully. \r\n ");
    }
    else
    {
        printf("Error occurred connecting %s \r\n ", WLAN_CONNECT_WPA_SSID);
        ret = -1;
    }

    if(NULL != externCallback)
    {
        externCallback(ret);
    }
}
