PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += ble_dis.c

APPLICATION_SRCS += simple_ble.c
APPLICATION_SRCS += simple_adv.c
APPLICATION_SRCS += led.c
APPLICATION_SRCS += device_info_service.c

LIBRARY_PATHS += ../../include
SOURCE_PATHS += ../../src

SOFTDEVICE_MODEL = s110

USE_BOOTLOADER = 1

NRF_BASE_PATH ?= ../..
include $(NRF_BASE_PATH)/make/Makefile
