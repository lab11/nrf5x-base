PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += led.c

DEVICE = NRF51

SOFTDEVICE_MODEL = s110

SDK_VERSION = 9
NRF_BASE_PATH ?= ../..

LIBRARY_PATHS += .  ../../include
SOURCE_PATHS += ../../src

include $(NRF_BASE_PATH)/make/Makefile
