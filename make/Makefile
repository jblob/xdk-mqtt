# Definition of the folder structure and the paths

# The variable BCDS_BASE_DIR should point to the root folder where the BCDS
# software package is installed. This folder must contain the sub-folders:
# platform, xdk110, lib and tools (TODO: Check the list on completeness!).
# If this variable is already defined, for example using an environment
# variable or a superior makefile, then it will not be overridden by this
# makefile.
# The default value "../../../.." assumes that this makefile is placed in the
# folder xdk110/apps/<App Folder>/make where the BCDS_BASE_DIR is the parent of
# the xdk110 folder.
BCDS_BASE_DIR ?= $(XDK_SDK)

# Reference to the root folder of this application.
APPLICATION_DIR=..

# JSMN folder
JSMN_DIR = $(JSMN)
PAHO_PACKET_DIR = $(PAHO_PKT)

include $(BCDS_BASE_DIR)/xdk110/make/application.mk

PROJECTNAME = relayrxdk
override CFLAGS += $(CFLAGS_APP)
override ASMFLAGS += $(ASMFLAGS_APP)
override LDFLAGS += $(LDFLAGS_APP)

INCLUDEPATHS += \
	-I $(APPLICATION_DIR)/inc \
	-I $(JSMN_DIR) \
	-I $(PAHO_PACKET_DIR)

# Application Files :Add the Source file
APP_SRC += \
	$(APPLICATION_DIR)/src/main.c \
	$(APPLICATION_DIR)/src/mqtt.c \
	$(APPLICATION_DIR)/src/envdata.c \
	$(APPLICATION_DIR)/src/acceldata.c \
	$(APPLICATION_DIR)/src/lightdata.c \
	$(APPLICATION_DIR)/src/gyrodata.c \
	$(APPLICATION_DIR)/src/magnetodata.c \
	$(APPLICATION_DIR)/src/wifi.c \
	$(APPLICATION_DIR)/src/xdkbutton.c \
	$(APPLICATION_DIR)/src/leds.c \
	$(APPLICATION_DIR)/src/controls.c \
	$(APPLICATION_DIR)/src/tasks.c \
	$(APPLICATION_DIR)/src/sensordata.c \
	$(APPLICATION_DIR)/src/MQTTClient.c \
	$(APPLICATION_DIR)/src/MQTTXDK.c \
	$(JSMN_DIR)/jsmn.c \
	$(PAHO_PACKET_DIR)/MQTTPacket.c \
	$(PAHO_PACKET_DIR)/MQTTConnectClient.c \
	$(PAHO_PACKET_DIR)/MQTTDeserializePublish.c \
	$(PAHO_PACKET_DIR)/MQTTSubscribeClient.c \
	$(PAHO_PACKET_DIR)/MQTTSerializePublish.c

STARTUP_SRC  += $(BCDS_XDK110_DIR)/startup_efm32gg.S

# Create a directory for Application and startup file
APP_FILES = $(notdir $(APP_SRC) )
STARTUP_FILES = $(notdir $(STARTUP_SRC) )

# Make list of source paths,Create a object file
APP_PATHS = $(sort $(dir $(APP_SRC) ) )
STARTUP_PATHS = $(sort $(dir $(STARTUP_SRC) ) )
APP_OBJS = $(addprefix $(OBJ_DIR)/, $(APP_FILES:.c=.o))
STARTUP_OBJS = $(if $(STARTUP_SRC), $(addprefix $(OBJ_DIR)/, $(STARTUP_FILES:.S=.o)))
C_DEPS = $(addprefix $(OBJ_DIR)/, $(APP_FILES:.c=.d))
OBJS = $(APP_OBJS) $(STARTUP_OBJS)

vpath %.c $(APP_PATHS)
vpath %.S $(STARTUP_PATHS)

ifeq ($(COMPILER),SYS_COMPILER )
ELF_SIZE = $(QUOTE)arm-none-eabi-size$(QUOTE)
endif
ifeq ($(COMPILER),GIT_COMPILER )
ELF_SIZE = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-size$(QUOTE)
endif

FLASHWIN   = $(QUOTE)$(BCDS_TOOLS_DIR)/eACommander/V2.82/eACommander$(QUOTE)
FLASHNIX   = JLinkExe
FLASH_CMD_FILE = jlinkcmd.jlink
# set to correct version based on system: WIN or NIX for Linux/OSX
FLASH = $(FLASHNIX)

# relayr cloud address & credentials
CREDENTIALS_JSON = $(APPLICATION_DIR)/src/credentials.json
CREDENTIALS_HEADER = $(APPLICATION_DIR)/inc/credentials.h
MQTT_SERVER = mqtt.relayr.io
MQTT_PORT = 1883
SSID = <PUT_YOUR_WIFI_SSID_HERE!
PASS = <PUT_YOUR_WIFI_PASS_HERE!

# Default build is release build
all:      release

release:  CFLAGS += -DNDEBUG -O0 -fomit-frame-pointer
debug:    CFLAGS += -O0 -g $(DEBUG_FEATURES_CONFIG)

# select correct flashing target based on system: WIN or NIX for Linux/OSX
flash: all flashnix

debug:: createcredentials
		$(LIB_DEBUG)
debug::$(OUTPUT).bin

release:: createcredentials
		$(LIB_RELEASE)

release::$(OUTPUT).bin

createcredentials:
	@echo "Generating credentials."
	@$(APPLICATION_DIR)/make/gencredentials.py $(CREDENTIALS_JSON) $(CREDENTIALS_HEADER) $(MQTT_SERVER) $(MQTT_PORT) \
	$(SSID) $(PASS)

# Create a output file using ld flags
$(OUTPUT).out: $(OBJS) $(LIBS)
	@echo [ LD ] $@
	@$(CC) $(OBJS) $(LIBS) $(LDFLAGS) -o $(OUTPUT).out


# Create a Application binary file
$(OUTPUT).bin: $(OUTPUT).out
	@echo [ BIN ] $@
	@$(OBJCOPY) -O binary $(OUTPUT).out $(OUTPUT).bin
	@echo [ HEX ] $(@:.bin=.hex)
	@$(OBJCOPY) -O ihex $(OUTPUT).out $(OUTPUT).hex
	@echo [ SIZE ]
	@$(ELF_SIZE) -B -x $(OUTPUT).hex


clean:
	 $(APP_CLEAN)
	 $(LIB_CLEAN)
	 $(RMFILES) $(CREDENTIALS_HEADER)
	 $(RMFILES) $(FLASH_CMD_FILE)
# including dependency files for target other than clean.
# So, make system will include all dependent files for other targets like, debug and release build.
ifneq (clean,$(findstring clean, $(MAKECMDGOALS)))
-include $(C_DEPS)
endif

flashwin: $(OUTPUT).bin
	@$(FLASH) --address $(XDK_APP_ADDRESS) -v -f $< -r

flashnix: $(OUTPUT).bin
	@echo "si 1" > $(FLASH_CMD_FILE)
	@echo "speed 4000" >> $(FLASH_CMD_FILE)
	@echo "device $(DEVICE)" >> $(FLASH_CMD_FILE)
	@echo "halt" >> $(FLASH_CMD_FILE)
	@echo "loadbin $(OUTPUT).hex $(XDK_APP_ADDRESS)" >> $(FLASH_CMD_FILE)
	@echo "r" >> $(FLASH_CMD_FILE)
	@echo "g" >> $(FLASH_CMD_FILE)
	@echo "q" >> $(FLASH_CMD_FILE)
	$(FLASH) -commanderscript $(FLASH_CMD_FILE)

