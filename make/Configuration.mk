# Configuration parameters for building nrf applications
# Included by AppMakefile.mk

# Ensure that this file is only included once
ifndef CONFIGURATION_MAKEFILE
CONFIGURATION_MAKEFILE = 1


# ---- Build tools

# directory for built output
BUILDDIR ?= _build/

# Remove built-in rules and variables
# n.b. no-op for make --version < 4.0
MAKEFLAGS += -r
MAKEFLAGS += -R

# Toolchain programs
TOOLCHAIN := arm-none-eabi
AR := $(TOOLCHAIN)-ar
AS := $(TOOLCHAIN)-as
CC := $(TOOLCHAIN)-gcc
CXX := $(TOOLCHAIN)-g++
GDB := $(TOOLCHAIN)-gdb
LD := $(TOOLCHAIN)-gcc
OBJCOPY := $(TOOLCHAIN)-objcopy
OBJDUMP := $(TOOLCHAIN)-objdump
RANLIB := $(TOOLCHAIN)-ranlib
READELF := $(TOOLCHAIN)-readelf
SIZE := $(TOOLCHAIN)-size

# Git version
GIT_VERSION := $(shell git describe --abbrev=4 --always --tags)
BARE_VERSION := $(lastword $(subst v, , $(firstword $(subst -, ,$(GIT_VERSION)))))

# Pretty-printing rules
# If environment variable V is non-empty, be verbose
ifneq ($(V),)
Q=
TRACE_AR  =
TRACE_AS  =
TRACE_BIN =
TRACE_CC  =
TRACE_CXX =
TRACE_DEP =
TRACE_DIR =
TRACE_HEX =
TRACE_LD  =
TRACE_LST =
TRACE_SIZ =
else
Q=@
TRACE_AR  = @echo "  AR       " $@
TRACE_AS  = @echo "  AS       " $<
TRACE_BIN = @echo " BIN       " $@
TRACE_CC  = @echo "  CC       " $<
TRACE_CXX = @echo " CXX       " $<
TRACE_DEP = @echo " DEP       " $<
TRACE_DIR = @echo " DIR       " $@
TRACE_HEX = @echo " HEX       " $@
TRACE_LD  = @echo "  LD       " $@
TRACE_LST = @echo " LST       " $<
TRACE_SIZ = @echo " SIZE      " $<
endif


# ---- Nordic configuration

# Set defaults based on NRF_IC and check validity
ifdef NRF_IC
  ifeq ($(NRF_IC), nrf52832)
    NRF_MODEL = nrf52
    SOFTDEVICE_MODEL ?= s132
    RAM_KB ?= 64
    FLASH_KB ?= 512
    FULL_IC = nrf52832_xxaa
    CONFIGURATION_VARS += NRF52 ID_FLASH_LOCATION=0x7FFF8
    ID_FLASH_LOCATION=0x7FFF8
  else ifeq ($(NRF_IC), nrf52840)
    NRF_MODEL = nrf52
    SOFTDEVICE_MODEL ?= s140
    RAM_KB ?= 256
    FLASH_KB ?= 1024
    FULL_IC = nrf52840_xxaa
    CONFIGURATION_VARS += ID_FLASH_LOCATION=0xFFFF8
    ID_FLASH_LOCATION=0xFFFF8
  endif
else
  $(error NRF_IC unspecified. Add it to app Makefile!)
endif
FULL_IC_UPPER = $(shell echo $(FULL_IC) | tr a-z A-Z)
NRF_IC_UPPER = $(shell echo $(NRF_IC) | tr a-z A-Z)
CONFIGURATION_VARS += $(FULL_IC_UPPER)

ifdef BOARD
  ifeq ($(BOARD), pca10059)
    CONFIGURATION_VARS += BOARD_PCA10059
  endif
endif

# Default SDK and softdevice versions
SDK_VERSION ?= 15
ifeq ($(SDK_VERSION), 15)
  ifeq ($(SOFTDEVICE_MODEL), s132)
    SOFTDEVICE_VERSION = 6.1.1
  else ifeq ($(SOFTDEVICE_MODEL), s140)
    SOFTDEVICE_VERSION = 6.1.1
  else ifeq ($(SOFTDEVICE_MODEL), blank)
    SOFTDEVICE_VERSION = 0
    USE_BLE = 0 # can't have BLE without a softdevice
    # if we want to use the MBR to manage a bootloader without a softdevice:
    ifeq ($(USE_MBR), 1)
      MBR_VERSION = 2.4.1
    endif
  endif
endif
CONFIGURATION_VARS += SDK_VERSION_$(SDK_VERSION)

# Identify the linker script for this particular configuration
LINKER_SCRIPT ?= gcc_$(NRF_IC)_$(SOFTDEVICE_MODEL)_$(SOFTDEVICE_VERSION)_$(RAM_KB)_$(FLASH_KB).ld

# Default wireless configurations
# These ought to be defined in the application or board makefiles
USE_BLE ?= 1
USE_THREAD ?= 0
USE_GZP ?= 0
USE_GZLL ?= 0
USE_ESB ?= 0


# ---- Info dump

# Print some helpful info about what the user is compiling for
space :=
space +=
$(info BUILD OPTIONS:)
$(info $(space) Version     $(GIT_VERSION))
$(info $(space) Chip        $(NRF_IC))
$(info $(space) RAM         $(RAM_KB) kB)
$(info $(space) FLASH       $(FLASH_KB) kB)
$(info $(space) SDK         $(SDK_VERSION))
$(info $(space) SoftDevice  $(SOFTDEVICE_MODEL) $(SOFTDEVICE_VERSION))
$(info $(space) Board       $(BOARD))
$(info $(space))

# Dump configuration for verbose builds
ifneq ($(V),)
  $(info )
  $(info **************************************************)
  $(info NRF BUILD SYSTEM -- VERBOSE BUILD)
  $(info **************************************************)
  $(info Config:)
  $(info TOOLCHAIN=$(TOOLCHAIN))
  $(info CC=$(CC))
  $(info MAKEFLAGS=$(MAKEFLAGS))
  $(info )
  $(info $(CC) --version = $(shell $(CC) --version))
  $(info **************************************************)
  $(info )
endif


# ---- Compilation flags

#XXX: make sure the `check_override` script is being run
ifneq (,$(filter $(NRF_IC),nrf52832 nrf52840))
  CPUFLAGS += -mthumb
  CPUFLAGS += -mabi=aapcs
  CPUFLAGS += -mcpu=cortex-m4
  CPUFLAGS += -march=armv7e-m
  CPUFLAGS += -mfloat-abi=hard
  CPUFLAGS += -mfpu=fpv4-sp-d16
endif

CONFIGURATION_DEFINES = $(addprefix -D,$(CONFIGURATION_VARS))

#XXX: compare to tock and nrf to add new flags
override CFLAGS += \
    -std=gnu11\
    -c\
    $(CPUFLAGS)\
    -Wall\
    -Wextra\
    -Wno-date-time\
    -Wno-unused-parameter\
    -Werror=return-type\
    -Wno-expansion-to-defined\
    $(CONFIGURATION_DEFINES)\
    $(SDK_DEFINES)\
    -DGIT_VERSION=\"$(GIT_VERSION)\"\
    -DCONFIG_GPIO_AS_PINRESET\
    -s\
    -ffunction-sections\
    -fdata-sections\
    -fno-strict-aliasing\
    -fno-builtin\
    -fshort-enums\
    $(HEADER_INCLUDES)\
    -MD\
    -fomit-frame-pointer\
    #-flto\
    #-D$(FULL_IC_UPPER)\
    #-DSDK_VERSION_$(SDK_VERSION)\
    #-DSOFTDEVICE_$(SOFTDEVICE_MODEL)\
    #-DCONFIG_GPIO_AS_PINRESET\

#XXX: document this somewhere
override OPTIMIZATION_FLAG ?= -Os

#XXX: compare to tock and nrf to add new flags
override LDFLAGS += \
    $(CPUFLAGS)\
    -L $(NRF_BASE_DIR)/make/ld\
    -T $(LINKER_SCRIPT)\
    -Wl,--gc-sections\
    -Wl,-Map=$(BUILDDIR)$(OUTPUT_NAME).Map\
    $(LINKER_INCLUDES)\

override LDLIBS += \
    -lc\
    -lnosys\
    -lm\
    -lstdc++\

#XXX: compare to tock and nrf to add new flags
override OBJDUMP_FLAGS += \
    --disassemble-all\
    --source\
    --disassembler-options=force-thumb\
    -C\
    --section-headers


#XXX: it would be good to have a way to bring these back but only compile the user code with them (not sdk)
##################################################################################################
# Extra warning flags not enabled by Wall or Wextra.
#
# I read through the gcc manual and grabbed the ones that I thought might be
# interesting / useful. Then I grabbed that snippet below to find other things
# that were left out of the manual that may be worth adding. Below are all
# warnings and a short description supported by (arm-none-eabi)-gcc as of
# v6.2.1 - Pat

# http://stackoverflow.com/questions/11714827/
# List all supported warnings and their status:
#   gcc -Wall -Wextra -Q --help=warning
# Below are all warnings produced in an un-merged set of sorted lists
# broken into C/C++, and C only

# TODO(Pat) libnrfserialization noise with these, but I think they're useful
# and I want them back when I get a chance to clean that up.
#CFLAGS += -Wcast-qual #                # const char* -> char*
#CFLAGS += -Wswitch-default #           # switch w/out default (doesn't cover all cases) (maybe annoying?)
#CFLAGS += -Wstrict-prototypes #          # function defined w/out specifying argument types

override CFLAGS += -Wdate-time #                # warn if __TIME__, __DATE__, or __TIMESTAMP__ used
                                         # ^on b/c flashing assumes same code => no flash, these enforce
override CFLAGS += -Wfloat-equal #              # floats used with '=' operator, likely imprecise
override CFLAGS += -Wformat-nonliteral #        # can't check format string (maybe disable if annoying)
override CFLAGS += -Wformat-security #          # using untrusted format strings (maybe disable)
override CFLAGS += -Wformat-y2k #               # use of strftime that assumes two digit years
override CFLAGS += -Wno-implicit-fallthrough    # warning that's occurring in the sdk
override CFLAGS += -Winit-self #                # { int i = i }
override CFLAGS += -Wlogical-op #               # "suspicous use of logical operators in expressions" (a lint)
#override CFLAGS += -Wmissing-declarations #     # ^same? not sure how these differ
override CFLAGS += -Wno-missing-field-initializers # if init'ing struct w/out field names, warn if not all used
override CFLAGS += -Wmissing-format-attribute # # something looks printf-like but isn't marked as such
#override CFLAGS += -Wmissing-noreturn #         # __attribute__((noreturn)) like -> ! in Rust, should use it
override CFLAGS += -Wmultichar #                # use of 'foo' instead of "foo" (surpised not on by default?)
override CFLAGS += -Wpointer-arith #            # sizeof things not define'd (i.e. sizeof(void))
#override CFLAGS += -Wredundant-decls #          # { int i; int i; } (a lint)
override CFLAGS += -Wshadow #                   # int foo(int a) { int a = 1; } inner a shadows outer a
override CFLAGS += -Wtrampolines #              # attempt to generate a trampoline on the NX stack
#override CFLAGS += -Wunused-macros #            # macro defined in this file not used
#override CFLAGS += -Wunused-parameter #         # function parameter is unused aside from its declaration
override CFLAGS += -Wwrite-strings #            # { char* c = "foo"; c[0] = 'b' } <-- "foo" should be r/o

#CFLAGS += -Wabi -Wabi-tag              # inter-compiler abi issues
#CFLAGS += -Waggregate-return           # warn if things return struct's
#CFLAGS += -Wcast-align                 # { char *c; int *i = (int*) c}, 1 byte -> 4 byte align
#CFLAGS += -Wconversion                 # implicit conversion that may unexpectedly alter value
#                                         ^ A ton of these from syscalls I think, XXX look later
#CFLAGS += -Wdisabled-optimization      # gcc skipped an optimization for any of a thousand reasons
#CFLAGS += -Wdouble-promotion           # warn if float -> double implicitly XXX maybe?
#CFLAGS += -Wformat-signedness #        # { int i; printf("%d %u", i, i) } second bad (maybe annoying?)
#                                         ^ Too obnoxious when you want hex of an int
#CFLAGS += -Wfloat-conversion           # subset of -Wconversion
#CFLAGS += -Winline                     # something marked `inline` wasn't inlined
#CFLAGS += -Winvalid-pch                # bad precompiled header found in an include dir
#CFLAGS += -Wmissing-include-dirs -- XXX Didn't try, afriad could be annoying
#CFLAGS += -Woverlength-strings         # complier compat: strings > [509 C90, 4095 C99] chars
#CFLAGS += -Wpacked                     # struct with __attribute__((packed)) that does nothing
#CFLAGS += -Wpadded                     # padding added to a struct. Noisy for argument structs
#CFLAGS += -Wpedantic                   # strict ISO C/C++
#CFLAGS += -Wsign-conversion            # implicit integer sign conversions, part of -Wconversion
#CFLAGS += -Wstack-protector            # only if -fstack-protector, on by default, warn fn not protect
#CFLAGS += -Wsuggest-attribute=const    # does what it sounds like - removed due to noise
#CFLAGS += -Wsuggest-attribute=pure     # does what it sounds like - removed due to noise
#CFLAGS += -Wswitch-enum #              # switch of enum doesn't explicitly cover all cases
#                                         ^ annoying in practice, let default: do its job
#CFLAGS += -Wsystem-headers             # warnings from system headers
#CFLAGS += -Wtraditional                # stuff gcc allows that "traditional" C doesn't
#CFLAGS += -Wundef                      # undefined identifier is evaluated in an `#if' directive
#                                         ^ Lots of library #if SAMD || SMAR21 stuff
#                                           Should probably be ifdef, but too much noise
#CFLAGS += -Wunsafe-loop-optimizations  # compiler can't divine loop bounds XXX maybe interesting?
#CFLAGS += -Wvariadic-macros            # can't be used in ISO C
#CFLAGS += -Wvector-operation-performance # perf option not appropriate for these systems
#CFLAGS += -Wvla                  -- XXX Didn't try, but interested

# C-only warnings
#override CFLAGS += -Wbad-function-cast #          # not obvious when this would trigger, could drop if annoying
override CFLAGS += -Wjump-misses-init #           # goto or switch skips over a variable initialziation
#override CFLAGS += -Wmissing-prototypes #         # global fn defined w/out prototype (should be static or in .h)
override CFLAGS += -Wnested-externs #             # mis/weird-use of extern keyword
#override CFLAGS += -Wold-style-definition #       # this garbage: void bar (a) int a; { }

#CFLAGS += -Wunsuffixed-float-constants # # { float f=0.67; if(f==0.67) printf("y"); else printf("n"); } => n
#                                         ^ doesn't seem to work right? find_north does funny stuff

#CFLAGS += -Wtraditional-conversion #     # prototype causes a conversion different than w/o prototype (?)
#                                         ^ real noisy
#
# END WARNINGS
##################################################################################################


endif

