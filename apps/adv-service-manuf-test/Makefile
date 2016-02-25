PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_timer.c

APPLICATION_SRCS += simple_ble.c
APPLICATION_SRCS += simple_adv.c

LIBRARY_PATHS += ../../include
SOURCE_PATHS += ../../src

SOFTDEVICE_MODEL = s110

NRF_BASE_PATH ?= ../..
include $(NRF_BASE_PATH)/make/Makefile
