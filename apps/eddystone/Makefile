PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_timer.c

APPLICATION_SRCS += simple_ble.c
APPLICATION_SRCS += eddystone.c

DEVICE = NRF51
USE_SOFTDEVICE = s110

SDK_VERSION = 9
NRF_BASE_PATH ?= ../../
TEMPLATE_PATH ?= ../../nrf51-pure-gcc-setup/template/
SOFTDEVICE ?= ../../softdevice/s110_nrf51_8.0.0_softdevice.hex
SDK_PATH ?= ../../sdk/nrf51_sdk_9.0.0/

LIBRARY_PATHS += ../../include
SOURCE_PATHS += ../../src

CFLAGS = -Os
GDB_PORT_NUMBER = 2331

include $(NRF_BASE_PATH)/Makefile.include
include $(TEMPLATE_PATH)Makefile
