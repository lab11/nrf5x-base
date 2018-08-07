PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_error_weak.c
APPLICATION_SRCS += nrfx_saadc.c
APPLICATION_SRCS += nrfx_power.c
APPLICATION_SRCS += nrfx_uart.c
APPLICATION_SRCS += nrfx_uarte.c
APPLICATION_SRCS += nrfx_prs.c
APPLICATION_SRCS += nrf_drv_uart.c
APPLICATION_SRCS += app_fifo.c
APPLICATION_SRCS += app_uart_fifo.c
APPLICATION_SRCS += retarget.c
APPLICATION_SRCS += app_util_platform.c

NRF_IC = nrf52832_xxaa
NRF_MODEL = nrf52
#BOARD = pca10036

SDK_VERSION = 15
SOFTDEVICE_MODEL = s132

NRF_BASE_PATH ?= ../..

LIBRARY_PATHS += .  ../../include
SOURCE_PATHS += ../../src

include $(NRF_BASE_PATH)/make/Makefile
