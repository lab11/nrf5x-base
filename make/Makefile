################################################################################
## Makefile for compiling apps against the nRF51 SDK
################################################################################

################################################################################
## Usage
##
## In your application Makefile, set the following variables:
##   - APPLICATION_SRCS   : List of all .c files to be compiled.
##   - LIBRARY_PATHS      : List of directories with .h files
##   - SOURCE_PATHS       : List of directories with .c files
##   - SOFTDEVICE_MODEL   : s110 | s120 | s130 | s210 | s310 | or do not set for no softdevice
##
## Optional
##   - SOFTDEVICE_VERSION : Full version number of the softdevice.
##   - SOFTDEVICE         : Path to the softdevice to use
##   - START_CODE         : .s file to execute first
##   - SYSTEM_FILE        : Base nRF .c file.
##   - NRF_MODEL          : nrf51 | nrf52  : Set by the softdevice used
##   - BOARD              : Hardware board being used. Will get set as #define.
##   - RAM_KB             : Size of RAM on chip    : Defaults to 16
##   - FLASH_KB           : Size of flash on chip  : Defaults to 256
##   - SDK_VERSION        : Major version number of the SDK to use. Defaults to 10
##   - GDB_PORT_NUMBER    : Defaults to 2331
##
##
## Then at the end of the Makefile do something like:
##    NRF_BASE_PATH ?= ../../nrf5x-base
##    include $(NRF_BASE_PATH)/make/Makefile

# Helper for relative paths
BASE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))..

# Setup the compilers to use
TOOLCHAIN_PREFIX ?= arm-none-eabi
AS      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-as
CC      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-gcc
LD      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-gcc
OBJCOPY = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-objcopy
OBJDUMP = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-objdump
SIZE    = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-size
GDB     = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-gdb

# Guess nRF51 unless otherwise set
NRF_MODEL ?= nrf51
NRF_IC    ?= nrf51822

# Set default board
BOARD ?= BOARD_NULL

# Set hardware memory sizes
RAM_KB   ?= 16
FLASH_KB ?= 256

# Default to SDK 10
SDK_VERSION ?= 10

# Set this for using GDB
GDB_PORT_NUMBER ?= 2331

# Configure which stacks we need for various softdevices
ifeq ($(SOFTDEVICE_MODEL), s110)
    USE_BLE = 1
    SOFTDEVICE_VERSION ?= 8.0.0
    NRF_MODEL = nrf51
endif

ifeq ($(SOFTDEVICE_MODEL), s120)
    USE_BLE = 1
    SOFTDEVICE_VERSION ?= 2.1.0
    NRF_MODEL = nrf51
endif

ifeq ($(SOFTDEVICE_MODEL), s130)
    USE_BLE = 1
    SOFTDEVICE_VERSION ?= 2.0.0
    NRF_MODEL = nrf51
    SDK_VERSION = 11
endif

ifeq ($(SOFTDEVICE_MODEL), s132)
    USE_BLE = 1
    SOFTDEVICE_VERSION ?= 2.0.0-4.alpha
    NRF_MODEL = nrf52
    NRF_IC    = nrf52832
endif

ifeq ($(SOFTDEVICE_MODEL), s210)
    USE_ANT = 1
    SOFTDEVICE_VERSION ?= 5.0.0
    NRF_MODEL = nrf51
endif

ifeq ($(SOFTDEVICE_MODEL), s310)
    USE_BLE = 1
    USE_ANT = 1
    SOFTDEVICE_VERSION ?= 3.0.0
    NRF_MODEL = nrf51
endif

# Print some helpful info about what the user is compiling for
space :=
space +=
$(info BUILD OPTIONS:)
$(info $(space) SoftDevice  $(SOFTDEVICE_MODEL))
$(info $(space) SDK         $(SDK_VERSION))
$(info $(space) nRF         $(NRF_IC))
$(info $(space) RAM         $(RAM_KB) kB)
$(info $(space) FLASH       $(FLASH_KB) kB)
$(info $(space) Board       $(BOARD))
$(info $(space))

# Need capital letters for the device in GCC
DEVICE ?= $(shell echo $(NRF_MODEL) | tr a-z A-Z)

# Required files to compile
START_CODE ?= startup_$(NRF_MODEL).s
SYSTEM_FILE ?= system_$(NRF_MODEL).c

# Set the path to the softdevice
SOFTDEVICE ?= $(BASE_DIR)/softdevice/$(SOFTDEVICE_MODEL)_$(NRF_MODEL)_$(SOFTDEVICE_VERSION)_softdevice.hex

BOOTLOADER_REGION_START = $(addprefix 0x, $(shell echo "obase=16; (${FLASH_KB}-18)*1024" | bc ))
BOOTLOADER_SETTINGS_ADDRESS = $(addprefix 0x, $(shell echo "obase=16; (${FLASH_KB}-2)*1024" | bc))

# Need some common options for interacting with the chip over JTAG
JLINK_OPTIONS = -device $(NRF_IC) -if swd -speed 1000

# If not set in app makefile, lets optimize for size
CFLAGS += -Os
CFLAGS += -fomit-frame-pointer

# Add useful paths from nRF5x-base
LIBRARY_PATHS += $(BASE_DIR)/advertisement/
LIBRARY_PATHS += $(BASE_DIR)/devices/
LIBRARY_PATHS += $(BASE_DIR)/lib/
LIBRARY_PATHS += $(BASE_DIR)/peripherals/
LIBRARY_PATHS += $(BASE_DIR)/services/

SOURCE_PATHS += $(BASE_DIR)/advertisement/
SOURCE_PATHS += $(BASE_DIR)/devices/
SOURCE_PATHS += $(BASE_DIR)/lib/
SOURCE_PATHS += $(BASE_DIR)/peripherals/
SOURCE_PATHS += $(BASE_DIR)/services/
SOURCE_PATHS += $(BASE_DIR)/startup/

# Add paths for each SDK version
ifeq ($(NRF_MODEL), nrf51)
ifeq ($(SDK_VERSION), 11)

	# Set the path
	SDK_PATH ?= $(BASE_DIR)/sdk/nrf51_sdk_11.0.0/

	# Other knowns about the SDK paths
	SDK_INCLUDE_PATH   = $(SDK_PATH)components/
	SDK_SOURCE_PATH    = $(SDK_PATH)components/
	CMSIS_INCLUDE_PATH = $(SDK_PATH)components/toolchain/gcc/

	# Need to add the paths for all the directories in the SDK.
	# Note that we do not use * because some folders have conflicting files.
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)libraries/*/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/ble_flash/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/clock/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/common/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/delay/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/gpiote/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/hal/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/lpcomp/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/ppi/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/pstorage/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/pstorage/config/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/radio_config/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/rng/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/rtc/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/sdio/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/spi_master/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/spi_slave/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/swi/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/timer/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/twi_master/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/uart/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/wdt/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/validation/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_ext/*/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)device/)
	LIBRARY_PATHS += $(SDK_INCLUDE_PATH)toolchain/gcc/
	LIBRARY_PATHS += $(SDK_INCLUDE_PATH)toolchain/
	LIBRARY_PATHS += $(SDK_INCLUDE_PATH)toolchain/CMSIS/Include/

	SOURCE_PATHS += $(SDK_SOURCE_PATH)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)libraries/*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)drivers_nrf/*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)drivers_ext/*/)

ifdef SERIALIZATION_MODE
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/*/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/transport/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/common/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/hal/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/transport/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/transport/ser_phy/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/transport/ser_phy/config/)

	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/transport/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/common/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/hal/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/transport/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/transport/ser_phy/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/transport/ser_phy/config/)

# Add defines based on if we are "connectivity" or "application"
ifeq ($(SERIALIZATION_MODE), connectivity)
    CFLAGS += -DSER_CONNECTIVITY -DAPP_SCHEDULER_WITH_PAUSE -DBLE_STACK_SUPPORT_REQD -DHCI_TIMER2 -DSOFTDEVICE_PRESENT
endif
ifeq ($(SERIALIZATION_MODE), application)
    CFLAGS += -DSVCALL_AS_NORMAL_FUNCTION -DBLE_STACK_SUPPORT_REQD
endif
endif

ifdef USE_BLE
    # How many central/peripherals are defined changes how much memory the
    # softdevice requires. Change the amount of memory allotted in a custom ld
    # file if your configuration is different than default.

    ifeq ($(RAM_KB), 16)
      # limit 16 kB RAM nRFs to only act as peripherals. Doing otherwise
      # requires careful balancing of memory requirements and should be done
      # manually, not automatically
      CENTRAL_LINK_COUNT ?= 0
      PERIPHERAL_LINK_COUNT ?= 1
    else
      CENTRAL_LINK_COUNT ?= 1
      PERIPHERAL_LINK_COUNT ?= 1
    endif

    LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)ble/*/)
    LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)ble/ble_services/*/)
    SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)ble/*/)
    SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)ble/ble_services/*/)
    CFLAGS += -DBLE_STACK_SUPPORT_REQD -DSOFTDEVICE_PRESENT -DCENTRAL_LINK_COUNT=$(CENTRAL_LINK_COUNT) -DPERIPHERAL_LINK_COUNT=$(PERIPHERAL_LINK_COUNT)
endif

ifdef USE_ANT
    CFLAGS += -DANT_STACK_SUPPORT_REQD
endif

ifdef ENABLE_WIRELESS_DFU
    CFLAGS += -DENABLE_DFU
    APPLICATION_SRCS += bootloader_util.c
endif

ifdef SOFTDEVICE_MODEL
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/common/softdevice_handler/
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/nrf51

    SOURCE_PATHS += $(SDK_SOURCE_PATH)softdevice/common/softdevice_handler/
    SOURCE_PATHS += $(SDK_SOURCE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/
    SOURCE_PATHS += $(SDK_SOURCE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/nrf51

ifdef SERIALIZATION_MODE
	# Load the sources from the serialization library
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/middleware/
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/serializers/
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/

    SOURCE_PATHS += $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/middleware/
    SOURCE_PATHS += $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/serializers/
    SOURCE_PATHS += $(SDK_SOURCE_PATH)serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/
endif
else
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/nrf_soc_nosd/)
    SOFTDEVICE_MODEL = blank
endif

endif # SDK 11

ifeq ($(SDK_VERSION), 10)

	# Set the path
	SDK_PATH ?= $(BASE_DIR)/sdk/nrf51_sdk_10.0.0/

	# Other knowns about the SDK paths
	SDK_INCLUDE_PATH   = $(SDK_PATH)components/
	SDK_SOURCE_PATH    = $(SDK_PATH)components/
	CMSIS_INCLUDE_PATH = $(SDK_PATH)components/toolchain/gcc/

	# Need to add the paths for all the directories in the SDK.
	# Note that we do not use * because some folders have conflicting files.
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)libraries/*/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/ble_flash/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/clock/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/common/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/delay/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/gpiote/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/hal/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/lpcomp/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/ppi/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/pstorage/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/pstorage/config/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/radio_config/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/rng/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/rtc/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/sdio/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/spi_master/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/spi_slave/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/swi/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/timer/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/twi_master/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/uart/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/wdt/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/validation/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_ext/*/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)device/)
	LIBRARY_PATHS += $(SDK_INCLUDE_PATH)toolchain/gcc/
	LIBRARY_PATHS += $(SDK_INCLUDE_PATH)toolchain/

	SOURCE_PATHS += $(SDK_SOURCE_PATH)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)libraries/*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)drivers_nrf/*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)drivers_ext/*/)

ifdef SERIALIZATION_MODE
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/*/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/transport/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/common/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/hal/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/transport/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/transport/ser_phy/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/transport/ser_phy/config/)

	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/transport/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/common/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/hal/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/transport/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/transport/ser_phy/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/transport/ser_phy/config/)

# Add defines based on if we are "connectivity" or "application"
ifeq ($(SERIALIZATION_MODE), connectivity)
    CFLAGS += -DSER_CONNECTIVITY -DAPP_SCHEDULER_WITH_PAUSE -DBLE_STACK_SUPPORT_REQD -DHCI_TIMER2 -DSOFTDEVICE_PRESENT
endif
ifeq ($(SERIALIZATION_MODE), application)
    CFLAGS += -DSVCALL_AS_NORMAL_FUNCTION -DBLE_STACK_SUPPORT_REQD
endif
else
ifdef SOFTDEVICE_MODEL
CFLAGS += -DSOFTDEVICE_PRESENT
endif
endif

ifdef USE_BLE
    LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)ble/*/)
    LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)ble/ble_services/*/)
    SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)ble/*/)
    SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)ble/ble_services/*/)
    CFLAGS += -DBLE_STACK_SUPPORT_REQD
endif

ifdef USE_ANT
    CFLAGS += -DANT_STACK_SUPPORT_REQD
endif

ifdef ENABLE_WIRELESS_DFU
    CFLAGS += -DENABLE_DFU
    APPLICATION_SRCS += bootloader_util.c
endif

ifdef SOFTDEVICE_MODEL
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/common/softdevice_handler/
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/

    SOURCE_PATHS += $(SDK_SOURCE_PATH)softdevice/common/softdevice_handler/
    SOURCE_PATHS += $(SDK_SOURCE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/

ifdef SERIALIZATION_MODE
	# Load the sources from the serialization library
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/middleware/
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/serializers/
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/

    SOURCE_PATHS += $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/middleware/
    SOURCE_PATHS += $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/serializers/
    SOURCE_PATHS += $(SDK_SOURCE_PATH)serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/
endif
else
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/nrf_soc_nosd/)
    SOFTDEVICE_MODEL = blank
endif

endif # SDK 10


ifeq ($(SDK_VERSION), 9)

	# Set the path
	SDK_PATH ?= $(BASE_DIR)/sdk/nrf51_sdk_9.0.0/

	# Other knowns about the SDK paths
	SDK_INCLUDE_PATH   = $(SDK_PATH)components/
	SDK_SOURCE_PATH    = $(SDK_PATH)components/
	CMSIS_INCLUDE_PATH = $(SDK_PATH)components/toolchain/gcc/

	# Need to add the paths for all the directories in the SDK.
	# Note that we do not use * because some folders have conflicting files.
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)libraries/*/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/ble_flash/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/clock/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/common/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/gpiote/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/hal/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/lpcomp/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/ppi/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/pstorage/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/radio_config/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/rng/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/rtc/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/sdio/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/spi_master/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/spi_slave/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/swi/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/timer/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/twi_master/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/uart/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/wdt/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_ext/*/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)device/)
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/*/)
	LIBRARY_PATHS += $(SDK_INCLUDE_PATH)toolchain/gcc/
	LIBRARY_PATHS += $(SDK_INCLUDE_PATH)toolchain/

	SOURCE_PATHS += $(SDK_SOURCE_PATH)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)libraries/*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)drivers_nrf/*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)drivers_ext/*/)
	SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/*/)

ifdef USE_BLE
    LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)ble/*/)
    LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)ble/ble_services/*/)
    SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)ble/*/)
    SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)ble/ble_services/*/)
    CFLAGS += -DBLE_STACK_SUPPORT_REQD
endif

ifdef USE_ANT
    CFLAGS += -DANT_STACK_SUPPORT_REQD
endif

ifdef ENABLE_WIRELESS_DFU
    CFLAGS += -DENABLE_DFU
    APPLICATION_SRCS += bootloader_util.c
endif

ifdef SOFTDEVICE_MODEL
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/common/softdevice_handler/
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/
    SOURCE_PATHS += $(SDK_INCLUDE_PATH)softdevice/common/softdevice_handler/
    SOURCE_PATHS += $(SDK_INCLUDE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/
else
	LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/nrf_soc_nosd/)
    SOFTDEVICE_MODEL = blank
endif

endif # SDK 9
endif # nrf51


ifeq ($(SOFTDEVICE_MODEL), blank)
    SOFTDEVICE_VERSION = 0
endif

# Location for BLE Address if stored in Flash
ifeq ($(FLASH_KB), 256)
	# 256 KB Flash
	CFLAGS += -DBLEADDR_FLASH_LOCATION=0x0003FFF8
	BLEADDR_FLASH_LOCATION ?= 0x0003FFF8
else
	# Assume 128 KB Flash
	CFLAGS += -DBLEADDR_FLASH_LOCATION=0x0001FFF8
	BLEADDR_FLASH_LOCATION ?= 0x0001FFF8
endif

# Hardware and Firmware version numbers
HW_REVISION ?= none
FW_REVISION ?= none
ENABLE_DIS ?= false
ifneq ($(HW_REVISION), none)
    CFLAGS += -DHW_REVISION=\"$(HW_REVISION)\"
    ENABLE_DIS = true
endif
ifneq ($(FW_REVISION), none)
    CFLAGS += -DFW_REVISION=\"$(FW_REVISION)\"
    ENABLE_DIS = true
endif
ifeq ($(ENABLE_DIS), true)
    APPLICATION_SRCS += ble_dis.c
    APPLICATION_SRCS += device_info_service.c
endif

print-% : ; @echo $* = $($*)

LINKER_SCRIPT ?= $(BASE_DIR)/make/ld/gcc_$(NRF_MODEL)_$(SOFTDEVICE_MODEL)_$(SOFTDEVICE_VERSION)_$(RAM_KB)_$(FLASH_KB).ld
OUTPUT_NAME ?= $(addsuffix _$(SOFTDEVICE_MODEL), $(PROJECT_NAME))
DFU_OUTPUT_NAME ?= $(addsuffix _$(FLASH_KB), $(addsuffix _$(RAM_KB), $(addsuffix _$(SOFTDEVICE_MODEL), dfu)))

LIBRARY_INCLUDES = $(addprefix -I,$(LIBRARY_PATHS))
CMSIS_INCLUDE = $(addprefix -I,$(CMSIS_INCLUDE_PATH))

VPATH = $(SOURCE_PATHS)

OUTPUT_PATH ?= _build/

CPUFLAGS = -mthumb -mcpu=cortex-m0 -march=armv6-m
CFLAGS += -std=gnu99 -c $(CPUFLAGS) -Wall -D$(DEVICE) -D$(BOARD) -DSDK_VERSION_$(SDK_VERSION) -DSOFTDEVICE_$(SOFTDEVICE_MODEL)
CFLAGS += -s -ffunction-sections -fdata-sections
CFLAGS += -D$(shell echo $(SOFTDEVICE_MODEL) | tr a-z A-Z)
CFLAGS += $(CMSIS_INCLUDE) $(LIBRARY_INCLUDES) -MD
LDFLAGS += $(CPUFLAGS) -L $(BASE_DIR)/make/ld -T $(LINKER_SCRIPT) -Wl,--gc-sections -Wl,-Map=$(OUTPUT_PATH)$(OUTPUT_NAME).Map

DFUHEX = $(BASE_DIR)/dfu/_build/$(DFU_OUTPUT_NAME).hex
DFUELF = $(BASE_DIR)/dfu/_build/$(DFU_OUTPUT_NAME).elf
DFUBIN = $(BASE_DIR)/dfu/_build/$(DFU_OUTPUT_NAME).bin

HEX = $(OUTPUT_PATH)$(OUTPUT_NAME).hex
ELF = $(OUTPUT_PATH)$(OUTPUT_NAME).elf
BIN = $(OUTPUT_PATH)$(OUTPUT_NAME).bin

SRCS = $(SYSTEM_FILE) $(notdir $(APPLICATION_SRCS))
OBJS = $(addprefix $(OUTPUT_PATH), $(SRCS:.c=.o)) $(addprefix $(OUTPUT_PATH),$(APPLICATION_LIBS))
DEPS = $(addprefix $(OUTPUT_PATH), $(SRCS:.c=.d))
SRCS_AS = $(START_CODE)
OBJS_AS = $(addprefix $(OUTPUT_PATH), $(SRCS_AS:.s=.os))

### Verbosity control. Use  make V=1  to get verbose builds.

ifeq ($(V),1)
  PRINT_CC  =
  PRINT_LD  =
  PRINT_AR  =
  PRINT_AS  =
  PRINT_HEX =
  PRINT_BIN =
  PRINT_SIZ =
  Q=
else
  PRINT_CC  = @echo "  CC       " $<
  PRINT_LD  = @echo "  LD       " $(ELF)
  PRINT_AR  = @echo "  AR       " $@
  PRINT_AS  = @echo "  AS       " $<
  PRINT_HEX = @echo "  HEX      " $(HEX)
  PRINT_BIN = @echo "  BIN      " $(BIN)
  PRINT_SIZ = @echo "  SIZE     " $(ELF)
  Q=@
endif

ifdef ENABLE_WIRELESS_DFU
all: $(OBJS) $(OBJS_AS) $(HEX) bootloader
else
all: $(OBJS) $(OBJS_AS) $(HEX)
endif

debug: CFLAGS += -g
ifdef ENABLE_WIRELESS_DFU
debug: $(OBJS) $(OBJS_AS) $(HEX) bootloader
else
debug: $(OBJS) $(OBJS_AS) $(HEX)
endif

rebuild: clean all

ifeq ($(OS),Windows_NT)
include $(BASE_DIR)/make/Makefile.windows
else
include $(BASE_DIR)/make/Makefile.posix
endif

$(HEX): $(OBJS) $(OBJS_AS)
	$(PRINT_LD)
	$(Q)$(LD) $(LDFLAGS) $(OBJS_AS) $(OBJS) -o $(ELF)
	$(PRINT_HEX)
	$(Q)$(OBJCOPY) -Oihex $(ELF) $(HEX)
	$(PRINT_BIN)
	$(Q)$(OBJCOPY) -Obinary $(ELF) $(BIN)
	$(PRINT_SIZ)
	$(Q)$(SIZE) $(ELF)

size: $(ELF)
	$(SIZE) $(ELF)

$(OUTPUT_PATH)%.o: %.c
	@$(MAKE_BUILD_FOLDER)
	$(PRINT_CC)
	$(Q)$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@

$(OUTPUT_PATH)%.os: %.s
	@$(MAKE_BUILD_FOLDER)
	$(PRINT_AS)
	$(Q)$(AS) $< -o $@

-include $(DEPS)

.PHONY: all clean rebuild size
