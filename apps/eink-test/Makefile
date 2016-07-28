PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_gpiote.c
APPLICATION_SRCS += led.c

APPLICATION_SRCS += nrf_drv_spi.c
APPLICATION_SRCS += nrf_drv_common.c
APPLICATION_SRCS += nrf_drv_gpiote.c
APPLICATION_SRCS += nrf_delay.c

# APPLICATION_SRCS += adxl362.c

NRF_BASE_PATH ?= ../..
LIBRARY_PATHS += . $(NRF_BASE_PATH)/devices ../../include
SOURCE_PATHS += $(NRF_BASE_PATH)/devices ../../src

SOURCE_PATHS += $(NRF_BASE_PATH)/devices/tcmp441/
LIBRARY_PATHS += $(NRF_BASE_PATH)/devices/tcmp441/
APPLICATION_SRCS += tcmp441.c

SOFTDEVICE_MODEL = s130
RAM_KB = 32


include $(NRF_BASE_PATH)/make/Makefile
