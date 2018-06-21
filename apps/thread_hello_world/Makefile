PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += app_error_weak.c

NRF_IC = nrf52840_xxaa
NRF_MODEL = nrf52

SDK_VERSION = 15

NRF_BASE_PATH ?= ../..

LIBRARY_PATHS += .  ../../include
SOURCE_PATHS += ../../src

USE_THREAD = 1

include $(NRF_BASE_PATH)/make/Makefile
