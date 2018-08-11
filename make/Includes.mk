# Included folders and source files for building nrf applications
# Included by Configuration.mk

# ensure that this file is only included once
ifndef INCLUDES_MAKEFILE
INCLUDES_MAKEFILE = 1


LIBRARY_PATHS += $(NRF_BASE_DIR)/lib/
SOURCE_PATHS += $(NRF_BASE_DIR)/lib/




ifneq (,$(filter $(NRF_IC),nrf52832 nrf52840))
  ifeq ($(SDK_VERSION), 15)

    # Set the path
    ifdef USE_THREAD
      SDK_ROOT = $(NRF_BASE_DIR)/sdk/nrf5_sdk_15.0.0_thread/
    else
      SDK_ROOT = $(NRF_BASE_DIR)/sdk/nrf5_sdk_15.0.0/
    endif

    #XXX: add in sdk_config and app_config stuff here

    # Need to add the paths for all the directories in the SDK.
    # Note that we do not use * because some folders have conflicting files.
    LIBRARY_PATHS += $(SDK_ROOT)modules/nrfx/
    LIBRARY_PATHS += $(SDK_ROOT)modules/nrfx/mdk/
    LIBRARY_PATHS += $(SDK_ROOT)modules/nrfx/hal/
    LIBRARY_PATHS += $(SDK_ROOT)modules/nrfx/drivers/include/
    LIBRARY_PATHS += $(SDK_ROOT)integration/nrfx/
    LIBRARY_PATHS += $(SDK_ROOT)integration/nrfx/legacy/
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/libraries/*/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/libraries/*/src/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/libraries/crypto/backend/*/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/adc/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/ble_flash/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/clock/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/common/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/delay/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/gpiote/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/hal/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/lpcomp/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/ppi/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/pstorage/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/pstorage/config/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/radio_config/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/rng/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/rtc/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/saadc/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/sdio/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/spi_master/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/spi_slave/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/swi/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/timer/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/twi_master/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/uart/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/wdt/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/validation/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_ext/*/)
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/device/)
    LIBRARY_PATHS += $(SDK_ROOT)external/fprintf/
    LIBRARY_PATHS += $(SDK_ROOT)external/segger_rtt/
    LIBRARY_PATHS += $(wildcard $(SDK_ROOT)external/*/include/)
    LIBRARY_PATHS += $(SDK_ROOT)components/toolchain/gcc/
    LIBRARY_PATHS += $(SDK_ROOT)components/toolchain/
    LIBRARY_PATHS += $(SDK_ROOT)components/toolchain/cmsis/include/
    LIBRARY_PATHS += $(SDK_ROOT)components/softdevice/common

    SOURCE_PATHS += $(SDK_ROOT)components/
    SOURCE_PATHS += $(SDK_ROOT)modules/nrfx/
    SOURCE_PATHS += $(SDK_ROOT)modules/nrfx/mdk/
    SOURCE_PATHS += $(SDK_ROOT)modules/nrfx/hal/
    SOURCE_PATHS += $(SDK_ROOT)modules/nrfx/drivers/src/
    SOURCE_PATHS += $(SDK_ROOT)modules/nrfx/drivers/src/prs/
    SOURCE_PATHS += $(SDK_ROOT)integration/nrfx/legacy/
    SOURCE_PATHS += $(wildcard $(SDK_ROOT)modules/nrfx/*/)
    SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/*/)
    SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/libraries/*/)
    SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/libraries/*/src/)
    SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/libraries/crypto/backend/*/)
    SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/*/)
    SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/drivers_ext/*/)
    SOURCE_PATHS += $(SDK_ROOT)components/toolchain/gcc/
    SOURCE_PATHS += $(SDK_ROOT)components/softdevice/common
    SOURCE_PATHS += $(SDK_ROOT)external/fprintf/
    SOURCE_PATHS  += $(SDK_ROOT)external/segger_rtt/

    ifdef SERIALIZATION_MODE
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/serialization/*/)
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/)
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/common/)
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/hal/)
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/transport/)
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/)
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/ser_phy/)
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/ser_phy/config/)

      SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/*/)
      SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/)
      SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/common/)
      SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/hal/)
      SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/transport/)
      SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/)
      SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/ser_phy/)
      SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/serialization/common/transport/ser_phy/config/)

      # Add defines based on if we are "connectivity" or "application"
      ifeq ($(SERIALIZATION_MODE), connectivity)
        INCLUDES_CFLAGS += -DSER_CONNECTIVITY -DAPP_SCHEDULER_WITH_PAUSE -DBLE_STACK_SUPPORT_REQD -DHCI_TIMER2 -DSOFTDEVICE_PRESENT
      endif
      ifeq ($(SERIALIZATION_MODE), application)
        INCLUDES_CFLAGS += -DSVCALL_AS_NORMAL_FUNCTION -DBLE_STACK_SUPPORT_REQD
      endif
    endif

    #XXX: do I define this anywhere?
    USE_BLE = 1
    #XXX: fix this
    ifdef USE_BLE
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/ble/*/)
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/ble/ble_services/*/)
      SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/ble/*/)
      SOURCE_PATHS += $(wildcard $(SDK_ROOT)components/ble/ble_services/*/)
      INCLUDES_CFLAGS += -DBLE_STACK_SUPPORT_REQD -DSOFTDEVICE_PRESENT -DNRF_SD_BLE_API_VERSION=6
    endif

    ifneq ($(SOFTDEVICE_MODEL), blank)
      LIBRARY_PATHS += $(SDK_ROOT)components/softdevice/common/softdevice_handler/
      LIBRARY_PATHS += $(SDK_ROOT)components/softdevice/$(SOFTDEVICE_MODEL)/headers/
      LIBRARY_PATHS += $(SDK_ROOT)components/softdevice/$(SOFTDEVICE_MODEL)/headers/nrf52

      SOURCE_PATHS += $(SDK_ROOT)components/softdevice/common/softdevice_handler/
      SOURCE_PATHS += $(SDK_ROOT)components/softdevice/$(SOFTDEVICE_MODEL)/headers/
      SOURCE_PATHS += $(SDK_ROOT)components/softdevice/$(SOFTDEVICE_MODEL)/headers/nrf52

      ifdef SERIALIZATION_MODE
        # Load the sources from the serialization library
        LIBRARY_PATHS += $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/middleware/
        LIBRARY_PATHS += $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/serializers/
        LIBRARY_PATHS += $(SDK_ROOT)components/serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/

        SOURCE_PATHS += $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/middleware/
        SOURCE_PATHS += $(SDK_ROOT)components/serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/serializers/
        SOURCE_PATHS += $(SDK_ROOT)components/serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/
      endif
    else
      LIBRARY_PATHS += $(wildcard $(SDK_ROOT)components/drivers_nrf/nrf_soc_nosd/)
    endif

    ifdef USE_THREAD
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

      LIBRARY_PATHS += $(SDK_ROOT)external/openthread/include/

      INCLUDES_CFLAGS += -DOPENTHREAD_ENABLE_APPLICATION_COAP -DOPENTHREAD_MTD=1
    endif

  endif # SDK 15
endif # nrf52


# Set the Softdevice path
SOFTDEVICE_PATH ?= $(SDK_ROOT)/components/softdevice/$(SOFTDEVICE_MODEL)/hex/$(SOFTDEVICE_MODEL)_nrf52_$(SOFTDEVICE_VERSION)_softdevice.hex


LIBRARY_INCLUDES = $(addprefix -I,$(LIBRARY_PATHS))

# Directories make searches for prerequisites
VPATH = $(SOURCE_PATHS)

#XXX: design some way to combine board, library, and application sources
#XXX: v this is a hack
SRCS = $(notdir $(APPLICATION_SRCS)) system_nrf52.c
OBJS = $(addprefix $(BUILDDIR), $(SRCS:.c=.o))
DEPS = $(addprefix $(BUILDDIR), $(SRCS:.c=.d))
#XXX: specify this within the context of sdk15
SRCS_AS = gcc_startup_nrf52.S
OBJS_AS = $(addprefix $(BUILDDIR), $(SRCS_AS:.S=.os))

endif

