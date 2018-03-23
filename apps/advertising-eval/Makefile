PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += app_util_platform.c
APPLICATION_SRCS += nrf_drv_common.c
APPLICATION_SRCS += nrf_delay.c
APPLICATION_SRCS += led.c

APPLICATION_SRCS += simple_ble.c
APPLICATION_SRCS += simple_adv.c

NRF_MODEL = nrf51
SDK_VERSION = 10
SOFTDEVICE_MODEL = s110

LIBRARY_PATHS += . ../../include
SOURCE_PATHS += ../../src

NRF_BASE_PATH ?= ../..
include $(NRF_BASE_PATH)/make/Makefile
