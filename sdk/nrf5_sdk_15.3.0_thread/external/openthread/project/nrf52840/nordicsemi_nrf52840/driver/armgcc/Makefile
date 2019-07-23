PROJECT_NAME     := ot_nrf52840_libnordicsemi_nrf52840_radio_driver
TARGETS          := libnordicsemi_nrf52840_radio_driver
OUTPUT_DIRECTORY := _build

SDK_ROOT := ../../../../../../..
PROJ_DIR := ../../..


# Source files common to all targets
SRC_FILES += \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/ack_generator/nrf_802154_ack_data.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/ack_generator/nrf_802154_ack_generator.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_ack_pending_bit.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/platform/clock/nrf_802154_clock_sdk.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_core.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_core_hooks.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_critical_section.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/nrf_802154_csma_ca.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_debug.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/nrf_802154_delayed_trx.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/ack_generator/nrf_802154_enh_ack_generator.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/nrf_802154_filter.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/nrf_802154_frame_parser.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/platform/hp_timer/nrf_802154_hp_timer.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/ack_generator/nrf_802154_imm_ack_generator.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_notification_direct.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_pib.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/nrf_802154_precise_ack_timeout.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_priority_drop_direct.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_request_direct.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_revision.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/rsch/nrf_802154_rsch.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/rsch/nrf_802154_rsch_crit_sect.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_rssi.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_rx_buffer.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/nrf_802154_timer_coord.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/timer_scheduler/nrf_802154_timer_sched.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/fem/nrf_fem_control.c \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/rsch/raal/single_phy/single_phy.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/dependencies \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/systick \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/include/mbedtls \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/nrfx/hal \
  $(PROJ_DIR) \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/softdevice/s140/headers/nrf52 \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/usb/class/cdc \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/include \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/delay \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/nrf_cc310/include \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/utf_converter \
  $(PROJ_DIR)/../openthread/examples/platforms/nrf52840 \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/hal \
  $(PROJ_DIR)/../openthread/third_party/mbedtls \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/common \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/rsch/raal/softdevice \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/usb/config \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/crypto \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/nrfx/mdk \
  $(PROJ_DIR)/../openthread/include \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/usbd \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/softdevice/s140/headers \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/rsch \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/usb \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/nrfx/soc \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/rsch/raal \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/usb/class/cdc/acm \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/atfifo \
  $(PROJ_DIR)/../openthread/src/core \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/cmsis \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/clock \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/power \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/atomic \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/app_error \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/nrfx/drivers/include \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/drivers/radio/mac_features/ack_generator \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/nrfx/drivers \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/nrfx \

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -Os -g1
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
CFLAGS += -DOPENTHREAD_CONFIG_ENABLE_SLAAC=1
CFLAGS += -DOPENTHREAD_CONFIG_LOG_OUTPUT=OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED
CFLAGS += -DOPENTHREAD_PROJECT_CORE_CONFIG_FILE="\"openthread-core-nrf52840-config.h\""
CFLAGS += -DRAAL_SINGLE_PHY=1
CFLAGS += -DUART_AS_SERIAL_TRANSPORT=1
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
CXXFLAGS += $(OPT) -fno-rtti -fno-exceptions
# Assembler flags common to all targets
ASMFLAGS += -g1
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
ASMFLAGS += -DOPENTHREAD_CONFIG_ENABLE_SLAAC=1
ASMFLAGS += -DOPENTHREAD_CONFIG_LOG_OUTPUT=OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED
ASMFLAGS += -DOPENTHREAD_PROJECT_CORE_CONFIG_FILE="\"openthread-core-nrf52840-config.h\""
ASMFLAGS += -DRAAL_SINGLE_PHY=1
ASMFLAGS += -DUART_AS_SERIAL_TRANSPORT=1
ASMFLAGS += -D_BSD_SOURCE=1
ASMFLAGS += -D_DEFAULT_SOURCE=1


libnordicsemi_nrf52840_radio_driver: CFLAGS += -D__HEAP_SIZE=4096
libnordicsemi_nrf52840_radio_driver: CFLAGS += -D__STACK_SIZE=4096
libnordicsemi_nrf52840_radio_driver: ASMFLAGS += -D__HEAP_SIZE=4096
libnordicsemi_nrf52840_radio_driver: ASMFLAGS += -D__STACK_SIZE=4096

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm -lstdc++


.PHONY: default help

# Default target - first one defined
default: libnordicsemi_nrf52840_radio_driver

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		libnordicsemi_nrf52840_radio_driver

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(call define_library, $(TARGETS), $(PROJ_DIR)/nordicsemi_nrf52840/driver/armgcc/libnordicsemi-nrf52840-radio-driver.a)

define create_library
@echo Creating library: $($@)
$(NO_ECHO)$(AR) $($@) $^
@echo Done
endef
libnordicsemi_nrf52840_radio_driver:
	$(create_library)

SDK_CONFIG_FILE := ../config/sdk_config.h
CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar
sdk_config:
	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)
