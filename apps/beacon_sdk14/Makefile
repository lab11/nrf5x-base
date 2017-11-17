PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_error_weak.c
APPLICATION_SRCS += nrf_drv_common.c
APPLICATION_SRCS += led.c
APPLICATION_SRCS += nrf_sdh.c
APPLICATION_SRCS += nrf_sdh_ble.c
APPLICATION_SRCS += nrf_sdh_soc.c
APPLICATION_SRCS += nrf_section_iter.c
APPLICATION_SRCS += app_util_platform.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += nrf_pwr_mgmt.c

NRF_MODEL = nrf52840

SDK_VERSION = 14
SOFTDEVICE_MODEL = s140

NRF_BASE_PATH ?= ../..

LIBRARY_PATHS += .  ../../include
SOURCE_PATHS += ../../src

include $(NRF_BASE_PATH)/make/Makefile
