PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_error_weak.c
APPLICATION_SRCS += nrf_sdh.c
APPLICATION_SRCS += nrf_section_iter.c
APPLICATION_SRCS += nrf_sdh_ble.c
APPLICATION_SRCS += app_util_platform.c

NRF_IC = nrf52840_xxaa

SDK_VERSION = 15
SOFTDEVICE_MODEL = s140

NRF_BASE_PATH ?= ../..

LIBRARY_PATHS += .  ../../include
SOURCE_PATHS += ../../src

include $(NRF_BASE_PATH)/make/Makefile
