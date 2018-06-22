PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_error_weak.c
APPLICATION_SRCS += app_util_platform.c
APPLICATION_SRCS += nrfx_power.c
APPLICATION_SRCS += nrfx_uart.c
APPLICATION_SRCS += nrfx_uarte.c
APPLICATION_SRCS += nrfx_timer.c
APPLICATION_SRCS += nrfx_clock.c
APPLICATION_SRCS += nrfx_power_clock.c
APPLICATION_SRCS += nrfx_prs.c
APPLICATION_SRCS += nrf_nvmc.c
APPLICATION_SRCS += nrf_drv_uart.c
APPLICATION_SRCS += nrf_drv_clock.c
APPLICATION_SRCS += app_fifo.c
APPLICATION_SRCS += app_uart_fifo.c
APPLICATION_SRCS += retarget.c

APPLICATION_SRCS += SEGGER_RTT.c
APPLICATION_SRCS += SEGGER_RTT_Syscalls_GCC.c
APPLICATION_SRCS += SEGGER_RTT_printf.c

NRF_IC = nrf52840_xxaa
SDK_VERSION = 15

LIBRARY_PATHS += .  ../../include
SOURCE_PATHS += ../../src

USE_THREAD = 1

NRF_BASE_PATH ?= ../..
include $(NRF_BASE_PATH)/make/Makefile
