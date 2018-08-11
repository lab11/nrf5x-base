PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_error_weak.c

NRF_IC = nrf52832
#BOARD = pca10036

SDK_VERSION = 15
SOFTDEVICE_MODEL = s132

NRF_BASE_DIR ?= ../..

LIBRARY_PATHS += .  ../../include
SOURCE_PATHS += ../../src

include $(NRF_BASE_DIR)/make/AppMakefile.mk
