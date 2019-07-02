PROJECT_NAME     := ot_nrf52811_libmbedcrypto_nrf52811
TARGETS          := libmbedcrypto_nrf52811
OUTPUT_DIRECTORY := _build

SDK_ROOT := ../../../../../../..
PROJ_DIR := ../../..


# Source files common to all targets
SRC_FILES += \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/aes.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/asn1parse.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/asn1write.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/base64.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/bignum.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ccm.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/cipher.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/cipher_wrap.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/cmac.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ctr_drbg.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/debug.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ecdh.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ecdsa.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ecjpake.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ecp.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ecp_curves.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/entropy.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/entropy_poll.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/md.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/md_wrap.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/memory_buffer_alloc.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/oid.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/pem.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/pk.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/pk_wrap.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/pkparse.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/platform.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/platform_util.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/sha256.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ssl_ciphersuites.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ssl_cli.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ssl_cookie.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ssl_srv.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ssl_ticket.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/ssl_tls.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/threading.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/x509.c \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/library/x509_crt.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(PROJ_DIR)/../openthread/third_party/mbedtls \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/include/mbedtls \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(PROJ_DIR) \
  $(PROJ_DIR)/../openthread/third_party/mbedtls/repo/include \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/crypto \
  $(PROJ_DIR)/../openthread/src/core \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/nrfx/mdk \
  $(PROJ_DIR)/../openthread/examples/platforms/nrf52811 \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/libraries/nrf_cc310/include \
  $(PROJ_DIR)/../openthread/third_party/NordicSemiconductor/cmsis \
  $(PROJ_DIR)/../openthread/include \

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -Os -g1
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DDISABLE_CC310=1
CFLAGS += -DFLOAT_ABI_SOFT
CFLAGS += -DHAVE_CONFIG_H
CFLAGS += -DMBEDTLS_CONFIG_FILE="\"mbedtls-config.h\""
CFLAGS += -DMBEDTLS_USER_CONFIG_FILE="\"nrf52811-mbedtls-config.h\""
CFLAGS += -DNRF52811_XXAA
CFLAGS += -DOPENTHREAD_CONFIG_ENABLE_SLAAC=1
CFLAGS += -DOPENTHREAD_CONFIG_LOG_OUTPUT=OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED
CFLAGS += -DOPENTHREAD_PROJECT_CORE_CONFIG_FILE="\"openthread-core-nrf52811-config.h\""
CFLAGS += -DUART_AS_SERIAL_TRANSPORT=1
CFLAGS += -D_BSD_SOURCE=1
CFLAGS += -D_DEFAULT_SOURCE=1
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
CFLAGS += -mfloat-abi=soft
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

# C++ flags common to all targets
CXXFLAGS += $(OPT) -fno-rtti -fno-exceptions
# Assembler flags common to all targets
ASMFLAGS += -g1
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=soft
ASMFLAGS += -DDISABLE_CC310=1
ASMFLAGS += -DFLOAT_ABI_SOFT
ASMFLAGS += -DHAVE_CONFIG_H
ASMFLAGS += -DMBEDTLS_CONFIG_FILE="\"mbedtls-config.h\""
ASMFLAGS += -DMBEDTLS_USER_CONFIG_FILE="\"nrf52811-mbedtls-config.h\""
ASMFLAGS += -DNRF52811_XXAA
ASMFLAGS += -DOPENTHREAD_CONFIG_ENABLE_SLAAC=1
ASMFLAGS += -DOPENTHREAD_CONFIG_LOG_OUTPUT=OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED
ASMFLAGS += -DOPENTHREAD_PROJECT_CORE_CONFIG_FILE="\"openthread-core-nrf52811-config.h\""
ASMFLAGS += -DUART_AS_SERIAL_TRANSPORT=1
ASMFLAGS += -D_BSD_SOURCE=1
ASMFLAGS += -D_DEFAULT_SOURCE=1


libmbedcrypto_nrf52811: CFLAGS += -D__HEAP_SIZE=4096
libmbedcrypto_nrf52811: CFLAGS += -D__STACK_SIZE=4096
libmbedcrypto_nrf52811: ASMFLAGS += -D__HEAP_SIZE=4096
libmbedcrypto_nrf52811: ASMFLAGS += -D__STACK_SIZE=4096

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm -lstdc++


.PHONY: default help

# Default target - first one defined
default: libmbedcrypto_nrf52811

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		libmbedcrypto_nrf52811

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(call define_library, $(TARGETS), $(PROJ_DIR)/mbedcrypto/cc310/armgcc/libmbedcrypto-nrf52811.a)

define create_library
@echo Creating library: $($@)
$(NO_ECHO)$(AR) $($@) $^
@echo Done
endef
libmbedcrypto_nrf52811:
	$(create_library)

SDK_CONFIG_FILE := ../config/sdk_config.h
CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar
sdk_config:
	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)
