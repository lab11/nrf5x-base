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

# Default to the newest SDK
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
    SOFTDEVICE_VERSION ?= 1.0.0
    NRF_MODEL = nrf51
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

# Need capital letters for the device in GCC
DEVICE ?= $(shell echo $(NRF_MODEL) | tr a-z A-Z)

# Required files to compile
START_CODE ?= startup_$(NRF_MODEL).s
SYSTEM_FILE ?= system_$(NRF_MODEL).c

# Set the path to the softdevice
SOFTDEVICE ?= $(BASE_DIR)/softdevice/$(SOFTDEVICE_MODEL)_$(NRF_MODEL)_$(SOFTDEVICE_VERSION)_softdevice.hex

# Need some common options for interacting with the chip over JTAG
JLINK_OPTIONS = -device $(NRF_IC) -if swd -speed 1000

# If not set in app makefile, lets optimize for size
CFLAGS ?= -Os

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

ifdef SOFTDEVICE_MODEL
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/common/softdevice_handler/
    LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/
    SOURCE_PATHS += $(SDK_INCLUDE_PATH)softdevice/common/softdevice_handler/
    SOURCE_PATHS += $(SDK_INCLUDE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/
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

print-% : ; @echo $* = $($*)

LINKER_SCRIPT ?= $(BASE_DIR)/make/ld/gcc_$(NRF_MODEL)_$(SOFTDEVICE_MODEL)_$(SOFTDEVICE_VERSION)_$(RAM_KB)_$(FLASH_KB).ld
OUTPUT_NAME ?= $(addsuffix _$(SOFTDEVICE_MODEL), $(PROJECT_NAME))

LIBRARY_INCLUDES = $(addprefix -I,$(LIBRARY_PATHS))
CMSIS_INCLUDE = $(addprefix -I,$(CMSIS_INCLUDE_PATH))

VPATH = $(SOURCE_PATHS)

OUTPUT_PATH = _build/

CPUFLAGS = -mthumb -mcpu=cortex-m0 -march=armv6-m
CFLAGS += -std=gnu99 -c $(CPUFLAGS) -Wall -D$(DEVICE) -D$(BOARD)
CFLAGS += -s -ffunction-sections -fdata-sections
CFLAGS += $(CMSIS_INCLUDE) $(LIBRARY_INCLUDES) -MD
LDFLAGS += $(CPUFLAGS) -L $(BASE_DIR)/make/ld -T $(LINKER_SCRIPT) -Wl,--gc-sections -Wl,-Map=$(OUTPUT_PATH)$(OUTPUT_NAME).Map

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


all: $(OBJS) $(OBJS_AS) $(HEX)

debug: CFLAGS += -g
debug: $(OBJS) $(OBJS_AS) $(HEX)


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
