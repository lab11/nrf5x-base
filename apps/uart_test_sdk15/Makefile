PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_error_weak.c
APPLICATION_SRCS += app_util_platform.c
APPLICATION_SRCS += nrfx_power.c
APPLICATION_SRCS += nrfx_uart.c
APPLICATION_SRCS += nrfx_uarte.c
APPLICATION_SRCS += nrfx_prs.c
APPLICATION_SRCS += nrf_drv_uart.c
APPLICATION_SRCS += app_fifo.c
APPLICATION_SRCS += app_uart_fifo.c
APPLICATION_SRCS += retarget.c

NRF_IC = nrf52832_xxaa
NRF_MODEL = nrf52

SDK_VERSION = 15
SOFTDEVICE_MODEL = s132

LIBRARY_PATHS += ./ ../../include/ ../../chips/
SOURCE_PATHS += ./ ../../chips/

NRF_BASE_PATH ?= ../../
include $(NRF_BASE_PATH)/make/Makefile
