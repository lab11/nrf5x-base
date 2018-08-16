PROJECT_NAME = $(shell basename "$(realpath ./)")

# Configurations
NRF_IC = nrf52832
SDK_VERSION = 15
SOFTDEVICE_MODEL = s132

# Source and header files
APP_HEADER_PATHS += .
APP_SOURCE_PATHS += .
APP_SOURCES = $(notdir $(wildcard ./*.c))
APP_SOURCES += app_timer.c
APP_SOURCES += app_util_platform.c
APP_SOURCES += nrfx_power.c
APP_SOURCES += nrfx_uart.c
APP_SOURCES += nrfx_uarte.c
APP_SOURCES += nrfx_prs.c
APP_SOURCES += nrf_drv_uart.c
APP_SOURCES += app_fifo.c
APP_SOURCES += app_uart_fifo.c
APP_SOURCES += retarget.c
APP_VARS += USE_APP_CONFIG

# Include board Makefile (if any)
#include $(NRF_BASE_DIR)/boards/<BOARD_NAME_HERE>

# Include main Makefile
NRF_BASE_DIR ?= ../../
include $(NRF_BASE_DIR)/make/AppMakefile.mk
