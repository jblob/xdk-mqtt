# XDK @ relayr cloud

This is an extension of XDK example app with easy connection to relayr cloud.
This app uses two libraries:
- Eclipse backed Paho MQTT (Embedded C version)
- Great little JSON parser - JSMN

# What you need

- Grab & install XDK Workbench (app tested with version 1.3)
- Clone Paho MQTT
- Clone JSMN
- Clone this app
- Linux/OSX only: Segger JLink debug probe

## Extra steps for Linux/OSX (needed only once):
- Set XDK_SDK environment variable to XDK SDK root directory (the one containing xdk110 folder)
- Set PAHO_PKT environment variable to \<PAHO_REPO\>/MQTTPacket/src directory
- Set JSMN environment variable to JSMN directory
- Set GCC_ARM environment variable to ARM gcc compiler installation
- Set SHELLNAMES and SH variables in \<XDK_SDK\>/xdk110/make/application.mk to your current shell (bash, zsh, etc.)
- Make sure that both GCC ARM and Segger JLink binaries are available from command line

# Running the app
1. Go to \<THIS_APP_REPO\>/make
2. Connect XDK with USB cable (and Segger JLink on Linux/OSX) and turn it on
3. Create a device on developer.relayr.io and save the credentials to \<THIS_APP_REPO\>/src/credentials.json
4. Add your WiFi SSID and password to \<THIS_APP_REPO\>/make/Makefile
5. Run: make (or "make flash" to build and flash)
6. Enjoy!

# Commands
Out of the box XDK will send measurements from all sensors to the cloud and reacts on following commands:

- Led control:
  - {"path" : "led", "command" : "\<ledcolor\>", "value" : "0 | 1 | 2"}
  - value: 0 - off, 1 - on, 2 - toggle
  - \<ledcolor\>: "red", "yellow", "orange"
- Period control:
  - {"path" : "per", "command" : "pub | sub", "value" : "0 - MAXINT32"}
  - command: pub - publish period, sub - subscribe period
  - value: number of milliseconds between consecutive sensor reads
- Sensor control:
  - {"path" : "sensor", "command" : "\<sensor\>", "value" : "1 | 0"}
  - values: 0 - off, 1 - on
  - \<sensor\>: "acc", "gyr", "env", "lgt", "mag"
