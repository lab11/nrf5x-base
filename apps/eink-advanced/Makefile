PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))

APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_gpiote.c
APPLICATION_SRCS += led.c

APPLICATION_SRCS += simple_ble.c
APPLICATION_SRCS += simple_adv.c

APPLICATION_SRCS += nrf_drv_spi.c
APPLICATION_SRCS += nrf_drv_common.c
APPLICATION_SRCS += nrf_drv_gpiote.c
APPLICATION_SRCS += nrf_delay.c

# APPLICATION_SRCS += adxl362.c

NRF_BASE_PATH ?= ../..
LIBRARY_PATHS += . $(NRF_BASE_PATH)/devices ../../include
LIBRARY_PATHS += ./libqrencode
SOURCE_PATHS += $(NRF_BASE_PATH)/devices ../../src

SOFTDEVICE_MODEL = s130
RAM_KB = 32

CFLAGS += -D__STATIC=''
LDFLAGS += --specs=nano.specs

include $(NRF_BASE_PATH)/make/Makefile
