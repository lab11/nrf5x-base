PROJECT_NAME     := openthread_lib_libopenthread_nrf52840_sdk_usb
TARGETS          := libopenthread_nrf52840_sdk_usb
OUTPUT_DIRECTORY := _build

SDK_ROOT := ../../../../../../../..
PROJ_DIR := ../../../../..


# Source files common to all targets
SRC_FILES += \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/alarm.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/diag.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/fem.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/flash.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/flash_nosd.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/logging.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/misc.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_ack_pending_bit.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/nrf_802154_ack_timeout.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/platform/clock/nrf_802154_clock_sdk.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_core.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_core_hooks.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_critical_section.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/nrf_802154_csma_ca.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_debug.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/nrf_802154_filter.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_notification_direct.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_pib.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_priority_drop_direct.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_request_direct.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_revision.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_rssi.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_rx_buffer.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/timer_scheduler/nrf_802154_timer_sched.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/fem/nrf_fem_control.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/platform.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/radio.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/random.c \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/raal/single_phy/single_phy.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/spi-slave.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/temp.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/uart.c \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840/usb-cdc-uart.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(PROJ_DIR)/openthread/examples/platforms \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/nrfx/hal \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/libraries/atfifo \
  $(PROJ_DIR)/openthread/examples/platforms/nrf52840 \
  $(PROJ_DIR)/openthread/include \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/common \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/dependencies \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/systick \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/softdevice/s140/headers \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/libraries/usb/class/cdc \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/segger_rtt \
  $(PROJ_DIR)/openthread/third_party/mbedtls/repo.patched/include/mbedtls \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/nrfx \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/usbd \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/nrfx/mdk \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/libraries/usb \
  $(PROJ_DIR)/openthread/src/core \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/libraries/usb/class/cdc/acm \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/libraries/nrf_cc310/include \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/raal \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/libraries/atomic \
  $(PROJ_DIR)/openthread/third_party/mbedtls \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/nrfx/drivers/include \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/libraries/crypto \
  $(PROJ_DIR)/openthread/third_party/mbedtls/repo.patched/include \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/power \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/libraries/usb/config \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/libraries/app_error \
  $(PROJ_DIR) \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/cmsis \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/nrfx/drivers \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/radio/hal \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/drivers/clock \
  $(PROJ_DIR)/openthread/third_party/NordicSemiconductor/nrfx/soc \

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -Os -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DENABLE_FEM=1
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DHAVE_CONFIG_H
CFLAGS += -DMBEDTLS_CONFIG_FILE="\"mbedtls-config.h\""
CFLAGS += -DMBEDTLS_USER_CONFIG_FILE="\"nrf52840-mbedtls-config.h\""
CFLAGS += -DNRF52840_XXAA
CFLAGS += -DNRF_802154_PROJECT_CONFIG="\"platform-config.h\""
CFLAGS += -DOPENTHREAD_CONFIG_LOG_OUTPUT=OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED
CFLAGS += -DOPENTHREAD_PROJECT_CORE_CONFIG_FILE="\"openthread-core-nrf52840-config.h\""
CFLAGS += -DRAAL_SINGLE_PHY=1
CFLAGS += -DUSB_CDC_AS_SERIAL_TRANSPORT=1
CFLAGS += -D_BSD_SOURCE=1
CFLAGS += -D_DEFAULT_SOURCE=1
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

# C++ flags common to all targets
CXXFLAGS += $(OPT)

# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DENABLE_FEM=1
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DHAVE_CONFIG_H
ASMFLAGS += -DMBEDTLS_CONFIG_FILE="\"mbedtls-config.h\""
ASMFLAGS += -DMBEDTLS_USER_CONFIG_FILE="\"nrf52840-mbedtls-config.h\""
ASMFLAGS += -DNRF52840_XXAA
ASMFLAGS += -DNRF_802154_PROJECT_CONFIG="\"platform-config.h\""
ASMFLAGS += -DOPENTHREAD_CONFIG_LOG_OUTPUT=OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED
ASMFLAGS += -DOPENTHREAD_PROJECT_CORE_CONFIG_FILE="\"openthread-core-nrf52840-config.h\""
ASMFLAGS += -DRAAL_SINGLE_PHY=1
ASMFLAGS += -DUSB_CDC_AS_SERIAL_TRANSPORT=1
ASMFLAGS += -D_BSD_SOURCE=1
ASMFLAGS += -D_DEFAULT_SOURCE=1


libopenthread_nrf52840_sdk_usb: CFLAGS += -D__HEAP_SIZE=4096
libopenthread_nrf52840_sdk_usb: CFLAGS += -D__STACK_SIZE=4096
libopenthread_nrf52840_sdk_usb: ASMFLAGS += -D__HEAP_SIZE=4096
libopenthread_nrf52840_sdk_usb: ASMFLAGS += -D__STACK_SIZE=4096

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm -lstdc++


.PHONY: default help

# Default target - first one defined
default: libopenthread_nrf52840_sdk_usb

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		libopenthread_nrf52840_sdk_usb

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(call define_library, $(TARGETS), $(PROJ_DIR)/lib/openthread_nrf52840/sdk/usb/armgcc/libopenthread-nrf52840-sdk-usb.a)

define create_library
@echo Creating library: $($@)
$(NO_ECHO)$(AR) $($@) $^
@echo Done
endef
libopenthread_nrf52840_sdk_usb:
	$(create_library)

SDK_CONFIG_FILE := ../config/sdk_config.h
CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar
sdk_config:
	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)
