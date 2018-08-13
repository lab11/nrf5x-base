# Included folders and source files for building nrf applications
# Included by Configuration.mk

# ensure that this file is only included once
ifndef INCLUDES_MAKEFILE
INCLUDES_MAKEFILE = 1


# ---- This repo's files
REPO_HEADER_PATHS += $(NRF_BASE_DIR)/lib/
REPO_SOURCE_PATHS += $(NRF_BASE_DIR)/lib/


# ---- SDK files

ifneq (,$(filter $(NRF_IC),nrf52832 nrf52840))
  ifeq ($(SDK_VERSION), 15)

    # Set the path
    ifeq ($(USE_THREAD), 1)
      SDK_ROOT = $(NRF_BASE_DIR)/sdk/nrf5_sdk_15.0.0_thread/
    else
      SDK_ROOT = $(NRF_BASE_DIR)/sdk/nrf5_sdk_15.0.0/
    endif

    # default files for ICs
    ifeq ($(NRF_IC),nrf52832)
      SDK_SOURCES += system_nrf52.c
      SDK_AS = gcc_startup_nrf52.S
    else ifeq ($(NRF_IC),nrf52840)
      SDK_SOURCES += system_nrf52840.c
      SDK_AS = gcc_startup_nrf52840.S
    endif

    # default C files necessary for any application
    #XXX: are there other C files that I can include here?
    SDK_SOURCES += app_error.c
    SDK_SOURCES += app_error_weak.c

    # Add paths for sdk-specific linker files
    SDK_LINKER_PATHS += $(SDK_ROOT)modules/nrfx/mdk/

    # Path for default sdk_config.h
    SDK_HEADER_PATHS += $(SDK_ROOT)config/$(NRF_IC)/config/

    # Need to add the paths for all the directories in the SDK.
    # Note that we do not use * because some folders have conflicting files.
    SDK_HEADER_PATHS += $(SDK_ROOT)modules/nrfx/
    SDK_HEADER_PATHS += $(SDK_ROOT)modules/nrfx/mdk/
    SDK_HEADER_PATHS += $(SDK_ROOT)modules/nrfx/hal/
    SDK_HEADER_PATHS += $(SDK_ROOT)modules/nrfx/drivers/include/
    SDK_HEADER_PATHS += $(SDK_ROOT)integration/nrfx/
    SDK_HEADER_PATHS += $(SDK_ROOT)integration/nrfx/legacy/
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/libraries/*/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/libraries/*/src/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/libraries/crypto/backend/*/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/adc/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/ble_flash/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/clock/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/common/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/delay/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/gpiote/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/hal/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/lpcomp/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/ppi/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/pstorage/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/pstorage/config/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/radio_config/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/rng/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/rtc/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/saadc/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/sdio/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/spi_master/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/spi_slave/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/swi/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/timer/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/twi_master/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/uart/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/wdt/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/validation/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_ext/*/)
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/device/)
    SDK_HEADER_PATHS += $(SDK_ROOT)external/fprintf/
    SDK_HEADER_PATHS += $(SDK_ROOT)external/segger_rtt/
    SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)external/*/include/)
    SDK_HEADER_PATHS += $(SDK_ROOT)components/toolchain/gcc/
    SDK_HEADER_PATHS += $(SDK_ROOT)components/toolchain/
    SDK_HEADER_PATHS += $(SDK_ROOT)components/toolchain/cmsis/include/
    SDK_HEADER_PATHS += $(SDK_ROOT)components/softdevice/common

    SDK_SOURCE_PATHS += $(SDK_ROOT)components/
    SDK_SOURCE_PATHS += $(SDK_ROOT)modules/nrfx/
    SDK_SOURCE_PATHS += $(SDK_ROOT)modules/nrfx/mdk/
    SDK_SOURCE_PATHS += $(SDK_ROOT)modules/nrfx/hal/
    SDK_SOURCE_PATHS += $(SDK_ROOT)modules/nrfx/drivers/src/
    SDK_SOURCE_PATHS += $(SDK_ROOT)modules/nrfx/drivers/src/prs/
    SDK_SOURCE_PATHS += $(SDK_ROOT)integration/nrfx/legacy/
    SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)modules/nrfx/*/)
    SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/*/)
    SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/libraries/*/)
    SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/libraries/*/src/)
    SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/libraries/crypto/backend/*/)
    SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/*/)
    SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/drivers_ext/*/)
    SDK_SOURCE_PATHS += $(SDK_ROOT)components/toolchain/gcc/
    SDK_SOURCE_PATHS += $(SDK_ROOT)components/softdevice/common
    SDK_SOURCE_PATHS += $(SDK_ROOT)external/fprintf/
    SDK_SOURCE_PATHS  += $(SDK_ROOT)external/segger_rtt/

    ifdef SERIALIZATION_MODE
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/serialization/*/)
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/)
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/common/)
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/hal/)
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/transport/)
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/)
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/ser_phy/)
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/ser_phy/config/)

      SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/*/)
      SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/)
      SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/common/)
      SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/hal/)
      SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/transport/)
      SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/)
      SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/ser_phy/)
      SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/ser_phy/config/)

      # Add defines based on if we are "connectivity" or "application"
      ifeq ($(SERIALIZATION_MODE), connectivity)
        SDK_VARS += SER_CONNECTIVITY APP_SCHEDULER_WITH_PAUSE BLE_STACK_SUPPORT_REQD HCI_TIMER2 SOFTDEVICE_PRESENT
      endif
      ifeq ($(SERIALIZATION_MODE), application)
        SDK_VARS += SVCALL_AS_NORMAL_FUNCTION BLE_STACK_SUPPORT_REQD
      endif
    endif

    ifeq ($(USE_BLE),1)
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/ble/*/)
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/ble/ble_services/*/)
      SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/ble/*/)
      SDK_SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/ble/ble_services/*/)
      SDK_VARS += BLE_STACK_SUPPORT_REQD SOFTDEVICE_PRESENT NRF_SD_BLE_API_VERSION=6
    endif

    ifneq ($(SOFTDEVICE_MODEL), blank)
      SDK_HEADER_PATHS += $(SDK_ROOT)components/softdevice/common/softdevice_handler/
      SDK_HEADER_PATHS += $(SDK_ROOT)components/softdevice/$(SOFTDEVICE_MODEL)/headers/
      SDK_HEADER_PATHS += $(SDK_ROOT)components/softdevice/$(SOFTDEVICE_MODEL)/headers/nrf52

      SDK_SOURCE_PATHS += $(SDK_ROOT)components/softdevice/common/softdevice_handler/
      SDK_SOURCE_PATHS += $(SDK_ROOT)components/softdevice/$(SOFTDEVICE_MODEL)/headers/
      SDK_SOURCE_PATHS += $(SDK_ROOT)components/softdevice/$(SOFTDEVICE_MODEL)/headers/nrf52

      ifdef SERIALIZATION_MODE
        # Load the sources from the serialization library
        SDK_HEADER_PATHS += $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/middleware/
        SDK_HEADER_PATHS += $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/serializers/
        SDK_HEADER_PATHS += $(SDK_ROOT)components/serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/

        SDK_SOURCE_PATHS += $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/middleware/
        SDK_SOURCE_PATHS += $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/serializers/
        SDK_SOURCE_PATHS += $(SDK_ROOT)components/serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/
      endif
    else
      SDK_HEADER_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/nrf_soc_nosd/)
    endif

    ifeq ($(USE_ANT),1)
      CFLAGS += -DANT_STACK_SUPPORT_REQD
    endif

    ifeq ($(USE_GZP),1)
      USE_GZLL = 1
      SDK_SOURCE_PATHS += $(SDK_SOURCE_PATH)proprietary_rf/gzll
      SDK_SOURCES += nrf_gzp.c
      SDK_SOURCES += nrf_gzp_device.c
      SDK_SOURCES += nrf_gzp_host.c
      SDK_SOURCES += nrf_gzp_host_nrf5x.c
    endif

    ifeq ($(USE_GZLL),1)
      SDK_HEADER_PATHS += $(SDK_INCLUDE_PATH)proprietary_rf/gzll
      LIBS += $(SDK_INCLUDE_PATH)proprietary_rf/gzll/gcc/gzll_nrf52_sd_resources_gcc.a
    endif

    ifeq ($(USE_ESB),1)
      SDK_SOURCE_PATHS += $(SDK_SOURCE_PATH)proprietary_rf/esb
      SDK_SOURCES += nrf_esb.c
    endif

    ifeq ($(USE_THREAD),1)
      ifdef THREAD_FTD
        THREAD_LIB_FILES += $(SDK_ROOT)external/openthread/lib/gcc/libopenthread-ftd.a
      else
        THREAD_LIB_FILES += $(SDK_ROOT)external/openthread/lib/gcc/libopenthread-mtd.a
      endif

      THREAD_LIB_FILES += $(SDK_ROOT)external/openthread/lib/gcc/libopenthread-platform-utils.a
      THREAD_LIB_FILES += $(SDK_ROOT)external/openthread/lib/gcc/libopenthread-nrf52840-sdk.a
      THREAD_LIB_FILES += $(SDK_ROOT)external/openthread/lib/gcc/libopenthread-diag.a
      THREAD_LIB_FILES += $(SDK_ROOT)external/openthread/lib/gcc/libmbedcrypto.a
      THREAD_LIB_FILES += $(SDK_ROOT)external/nrf_cc310/lib/libnrf_cc310_0.9.9.a
      LIBS += $(THREAD_LIB_FILES)

      SDK_HEADER_PATHS += $(SDK_ROOT)external/openthread/include/

      SDK_VARS += OPENTHREAD_ENABLE_APPLICATION_COAP OPENTHREAD_MTD=1
    endif

  endif # SDK 15
endif # nrf52


# ---- Create variables for Configuration use

# Location of softdevice
SOFTDEVICE_PATH ?= $(SDK_ROOT)/components/softdevice/$(SOFTDEVICE_MODEL)/hex/$(SOFTDEVICE_MODEL)_nrf52_$(SOFTDEVICE_VERSION)_softdevice.hex

# Flags for compiler
HEADER_INCLUDES = $(addprefix -I,$(SDK_HEADER_PATHS)) $(addprefix -I,$(REPO_HEADER_PATHS)) $(addprefix -I,$(BOARD_HEADER_PATHS)) $(addprefix -I,$(APP_HEADER_PATHS))
LINKER_INCLUDES = $(addprefix -L,$(SDK_LINKER_PATHS)) $(addprefix -L,$(BOARD_LINKER_PATHS))
SDK_DEFINES = $(addprefix -D,$(SDK_VARS)) $(addprefix -D,$(BOARD_VARS)) $(addprefix -D,$(APP_VARS))

# Directories make searches for prerequisites
VPATH = $(SDK_SOURCE_PATHS) $(REPO_SOURCE_PATHS) $(BOARD_SOURCE_PATHS) $(APP_SOURCE_PATHS)

SOURCES = $(notdir $(SDK_SOURCES)) $(notdir $(BOARD_SOURCES)) $(notdir $(APP_SOURCES))
OBJS = $(addprefix $(BUILDDIR), $(SOURCES:.c=.o))
DEBUG_OBJS = $(addprefix $(BUILDDIR), $(SOURCES:.c=.o-debug))
DEPS = $(addprefix $(BUILDDIR), $(SOURCES:.c=.d))

SOURCES_AS = $(notdir $(SDK_AS)) $(notdir $(BOARD_AS)) $(notdir $(APP_AS))
OBJS_AS = $(addprefix $(BUILDDIR), $(SOURCES_AS:.S=.os))
DEBUG_OBJS_AS = $(addprefix $(BUILDDIR), $(SOURCES_AS:.S=.os-debug))

endif
