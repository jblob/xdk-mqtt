# XDK @ relayr cloud

This is an extension of XDK example app with easy connection to relayr cloud.
This app uses two libraries as submodules:
- Eclipse backed Paho MQTT (Embedded C version)
- Great little JSON parser - JSMN

# What you need

- Grab & install XDK Workbench (app tested with version 1.5)
- Clone this app and init submodules - git sumbodule init --recursive
- Linux/OSX only: debug probe (currently supported: JTAG Lock Pick Tiny 2 via OpenOCD and Segger JLink)

## Extra steps for Linux/OSX (needed only once):
- Set XDK_SDK environment variable to XDK SDK root directory (the one containing xdk110 folder)
- Set GCC_ARM environment variable to ARM gcc compiler installation
- Set SHELLNAMES and SH variables in \<XDK_SDK\>/xdk110/make/application.mk to your current shell (bash, zsh, etc.)
- Make sure that both GCC ARM and Segger JLink/OpenOCD binaries are available from command line

# Running the app
1. Go to \<THIS_APP_REPO\>
2. Connect XDK with USB cable (and debug probe on Linux/OSX) and turn it on
3. Create a device on developer.relayr.io and save the credentials to \<THIS_APP_REPO\>/src/credentials.json
4. Add your WiFi SSID and password to \<THIS_APP_REPO\>/Makefile
5. Run: make (or "make flashsegger" or "make flashocd" to build and flash with appropriate debugger)
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
