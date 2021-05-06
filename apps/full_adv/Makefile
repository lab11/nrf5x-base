PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += bootloader_util.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += app_util_platform.c
APPLICATION_SRCS += nrf_drv_common.c
APPLICATION_SRCS += nrf_delay.c
APPLICATION_SRCS += led.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_error_weak.c

APPLICATION_SRCS += simple_ble.c
APPLICATION_SRCS += simple_adv.c

USE_NRF52 := $(filter nrf52,$(MAKECMDGOALS))

# Compile for nrf51 by default
SOFTDEVICE_MODEL = s130
SDK_VERSION = 11
NRF_MODEL = nrf51

ifeq ($(USE_NRF52), nrf52)
SOFTDEVICE_MODEL = s132
SDK_VERSION = 11
NRF_MODEL = nrf52
endif

# Use "make nrf52" to compile for nrf52 platform
nrf52: clean all

LIBRARY_PATHS += .
#./../include
SOURCE_PATHS += ../../src

NRF_BASE_PATH ?= ../..
include $(NRF_BASE_PATH)/make/Makefile
