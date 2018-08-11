# Configuration parameters for building nrf applications
# Included by AppMakefile.mk

# ensure that this file is only included once
ifndef CONFIGURATION_MAKEFILE
CONFIGURATION_MAKEFILE = 1

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

# Default port for GDB
GDB_PORT_NUMBER ?= 2331

# Set defaults based on NRF_IC and check validity
ifdef NRF_IC
  ifeq ($(NRF_IC), nrf52832)
    NRF_MODEL = nrf52
    SOFTDEVICE_MODEL ?= s132
    RAM_KB ?= 64
    FLASH_KB ?= 512
    FULL_IC = nrf52832_xxaa
    #XXX: might also have to define NRF52 to make things work
  else ifeq ($(NRF_IC), nrf52840)
    NRF_MODEL = nrf52
    SOFTDEVICE_MODEL ?= s140
    RAM_KB ?= 256
    FLASH_KB ?= 1024
    FULL_IC = nrf52840_xxaa
  endif
else
  $(error NRF_IC unspecified. Add it to app Makefile!)
endif
FULL_IC_UPPER = $(shell echo $(FULL_IC) | tr a-z A-Z)

# Default SDK and softdevice versions
SDK_VERSION ?= 15
ifeq ($(SDK_VERSION), 15)
  ifeq ($(SOFTDEVICE_MODEL), s132)
    SOFTDEVICE_VERSION = 6.0.0
  else ifeq ($(SOFTDEVICE_MODEL), s140)
    SOFTDEVICE_VERSION = 6.0.0
  else ifeq ($(SOFTDEVICE_MODEL), blank)
    SOFTDEVICE_VERSION = 0
  endif
endif

# Print some helpful info about what the user is compiling for
space :=
space +=
$(info BUILD OPTIONS:)
$(info $(space) Chip        $(NRF_IC))
$(info $(space) RAM         $(RAM_KB) kB)
$(info $(space) FLASH       $(FLASH_KB) kB)
$(info $(space) SDK         $(SDK_VERSION))
$(info $(space) SoftDevice  $(SOFTDEVICE_MODEL) $(SOFTDEVICE_VERSION))
$(info $(space) Board       $(BOARD))
$(info $(space))

# Need some common options for interacting with the chip over JTAG
JLINK_OPTIONS = -device $(FULL_IC) -if swd -speed 1000

# Identify the linker script for this particular configuration
LINKER_SCRIPT ?= gcc_$(NRF_IC)_$(SOFTDEVICE_MODEL)_$(SOFTDEVICE_VERSION)_$(RAM_KB)_$(FLASH_KB).ld

# Output filenames
OUTPUT_NAME ?= $(addsuffix _sdk$(SDK_VERSION)_$(SOFTDEVICE_MODEL), $(PROJECT_NAME))
HEX = $(BUILDDIR)$(OUTPUT_NAME).hex
ELF = $(BUILDDIR)$(OUTPUT_NAME).elf
BIN = $(BUILDDIR)$(OUTPUT_NAME).bin
LST = $(BUILDDIR)$(OUTPUT_NAME).lst
MAP = $(BUILDDIR)$(OUTPUT_NAME).Map



#XXX: make sure the `check_override` script is being run

ifneq (,$(filter $(NRF_IC),nrf52832 nrf52840))
  CPUFLAGS += -mthumb
  CPUFLAGS += -mabi=aapcs
  CPUFLAGS += -mcpu=cortex-m4
  CPUFLAGS += -march=armv7e-m
  CPUFLAGS += -mfloat-abi=hard
  CPUFLAGS += -mfpu=fpv4-sp-d16
endif

#XXX: compare to tock and nrf to add new flags
override CFLAGS += \
    -std=gnu11\
    -c\
    $(CPUFLAGS)\
    -Wall\
    -Wextra\
    -Wno-unused-parameter\
    -Werror=return-type\
    -Wno-expansion-to-defined\
    -D$(FULL_IC_UPPER)\
    -DSDK_VERSION_$(SDK_VERSION)\
    -DSOFTDEVICE_$(SOFTDEVICE_MODEL)\
    -DCONFIG_GPIO_AS_PINRESET\
    -s\
    -ffunction-sections\
    -fdata-sections\
    $(LIBRARY_INCLUDES)\
    -MD\
    -Os\
    -fomit-frame-pointer\

#XXX: make sure this points to mdk too
#XXX: compare to tock and nrf to add new flags
override LDFLAGS += \
    $(CPUFLAGS)\
    -L $(NRF_BASE_DIR)/make/ld\
    -T $(LINKER_SCRIPT)\
    -Wl,--gc-sections\
    -Wl,-Map=$(BUILDDIR)$(OUTPUT_NAME).Map\
    -lc\
    -lnosys\
    -lm\
    -lstdc++\
    -L $(NRF_BASE_DIR)/sdk/nrf5_sdk_15.0.0/modules/nrfx/mdk/
#XXX: ^ hack

#XXX: compare to tock and nrf to add new flags
override OBJDUMP_FLAGS += \
    --disassemble-all\
    --source\
    --disassembler-options=force-thumb\
    -C\
    --section-headers




#XXX: commented out for now. Bring these all back once we get things working
###################################################################################################
## Extra warning flags not enabled by Wall or Wextra.
##
## I read through the gcc manual and grabbed the ones that I thought might be
## interesting / useful. Then I grabbed that snippet below to find other things
## that were left out of the manual that may be worth adding. Below are all
## warnings and a short description supported by (arm-none-eabi)-gcc as of
## v6.2.1 - Pat
#
## http://stackoverflow.com/questions/11714827/
## List all supported warnings and their status:
##   gcc -Wall -Wextra -Q --help=warning
## Below are all warnings produced in an un-merged set of sorted lists
## broken into C/C++, C only, C++ only, other languages
#
## TODO(Pat) libnrfserialization noise with these, but I think they're useful
## and I want them back when I get a chance to clean that up.
##CPPFLAGS += -Wcast-qual #                # const char* -> char*
##CPPFLAGS += -Wswitch-default #           # switch w/out default (doesn't cover all cases) (maybe annoying?)
##CFLAGS += -Wstrict-prototypes #          # function defined w/out specifying argument types
#
#override CPPFLAGS += -Wdate-time #                # warn if __TIME__, __DATE__, or __TIMESTAMP__ used
#                                         # ^on b/c flashing assumes same code => no flash, these enforce
#override CPPFLAGS += -Wfloat-equal #              # floats used with '=' operator, likely imprecise
#override CPPFLAGS += -Wformat-nonliteral #        # can't check format string (maybe disable if annoying)
#override CPPFLAGS += -Wformat-security #          # using untrusted format strings (maybe disable)
#override CPPFLAGS += -Wformat-y2k #               # use of strftime that assumes two digit years
#override CPPFLAGS += -Winit-self #                # { int i = i }
#override CPPFLAGS += -Wlogical-op #               # "suspicous use of logical operators in expressions" (a lint)
#override CPPFLAGS += -Wmissing-declarations #     # ^same? not sure how these differ
#override CPPFLAGS += -Wmissing-field-initializers # if init'ing struct w/out field names, warn if not all used
#override CPPFLAGS += -Wmissing-format-attribute # # something looks printf-like but isn't marked as such
#override CPPFLAGS += -Wmissing-noreturn #         # __attribute__((noreturn)) like -> ! in Rust, should use it
#override CPPFLAGS += -Wmultichar #                # use of 'foo' instead of "foo" (surpised not on by default?)
#override CPPFLAGS += -Wpointer-arith #            # sizeof things not define'd (i.e. sizeof(void))
#override CPPFLAGS += -Wredundant-decls #          # { int i; int i; } (a lint)
#override CPPFLAGS += -Wshadow #                   # int foo(int a) { int a = 1; } inner a shadows outer a
#override CPPFLAGS += -Wtrampolines #              # attempt to generate a trampoline on the NX stack
#override CPPFLAGS += -Wunused-macros #            # macro defined in this file not used
#override CPPFLAGS += -Wunused-parameter #         # function parameter is unused aside from its declaration
#override CPPFLAGS += -Wwrite-strings #            # { char* c = "foo"; c[0] = 'b' } <-- "foo" should be r/o
#
##CPPFLAGS += -Wabi -Wabi-tag              # inter-compiler abi issues
##CPPFLAGS += -Waggregate-return           # warn if things return struct's
##CPPFLAGS += -Wcast-align                 # { char *c; int *i = (int*) c}, 1 byte -> 4 byte align
##CPPFLAGS += -Wconversion                 # implicit conversion that may unexpectedly alter value
##                                         ^ A ton of these from syscalls I think, XXX look later
##CPPFLAGS += -Wdisabled-optimization      # gcc skipped an optimization for any of a thousand reasons
##CPPFLAGS += -Wdouble-promotion           # warn if float -> double implicitly XXX maybe?
##CPPFLAGS += -Wformat-signedness #        # { int i; printf("%d %u", i, i) } second bad (maybe annoying?)
##                                         ^ Too obnoxious when you want hex of an int
##CPPFLAGS += -Wfloat-conversion           # subset of -Wconversion
##CPPFLAGS += -Winline                     # something marked `inline` wasn't inlined
##CPPFLAGS += -Winvalid-pch                # bad precompiled header found in an include dir
##CPPFLAGS += -Wmissing-include-dirs -- XXX Didn't try, afriad could be annoying
##CPPFLAGS += -Woverlength-strings         # complier compat: strings > [509 C90, 4095 C99] chars
##CPPFLAGS += -Wpacked                     # struct with __attribute__((packed)) that does nothing
##CPPFLAGS += -Wpadded                     # padding added to a struct. Noisy for argument structs
##CPPFLAGS += -Wpedantic                   # strict ISO C/C++
##CPPFLAGS += -Wsign-conversion            # implicit integer sign conversions, part of -Wconversion
##CPPFLAGS += -Wstack-protector            # only if -fstack-protector, on by default, warn fn not protect
##CPPFLAGS += -Wsuggest-attribute=const    # does what it sounds like - removed due to noise
##CPPFLAGS += -Wsuggest-attribute=pure     # does what it sounds like - removed due to noise
##CPPFLAGS += -Wswitch-enum #              # switch of enum doesn't explicitly cover all cases
##                                         ^ annoying in practice, let default: do its job
##CPPFLAGS += -Wsystem-headers             # warnings from system headers
##CPPFLAGS += -Wtraditional                # stuff gcc allows that "traditional" C doesn't
##CPPFLAGS += -Wundef                      # undefined identifier is evaluated in an `#if' directive
##                                         ^ Lots of library #if SAMD || SMAR21 stuff
##                                           Should probably be ifdef, but too much noise
##CPPFLAGS += -Wunsafe-loop-optimizations  # compiler can't divine loop bounds XXX maybe interesting?
##CPPFLAGS += -Wvariadic-macros            # can't be used in ISO C
##CPPFLAGS += -Wvector-operation-performance # perf option not appropriate for these systems
##CPPFLAGS += -Wvla                  -- XXX Didn't try, but interested
#
## C-only warnings
#override CFLAGS += -Wbad-function-cast #          # not obvious when this would trigger, could drop if annoying
#override CFLAGS += -Wjump-misses-init #           # goto or switch skips over a variable initialziation
#override CFLAGS += -Wmissing-prototypes #         # global fn defined w/out prototype (should be static or in .h)
#override CFLAGS += -Wnested-externs #             # mis/weird-use of extern keyword
#override CFLAGS += -Wold-style-definition #       # this garbage: void bar (a) int a; { }
#
##CFLAGS += -Wunsuffixed-float-constants # # { float f=0.67; if(f==0.67) printf("y"); else printf("n"); } => n
##                                         ^ doesn't seem to work right? find_north does funny stuff
#
##CFLAGS += -Wtraditional-conversion #     # prototype causes a conversion different than w/o prototype (?)
##                                         ^ real noisy
#
## CXX-only warnings
#override CXXFLAGS += -Wctor-dtor-privacy #        # unusable class b/c everything private and no friends
#override CXXFLAGS += -Wdelete-non-virtual-dtor #  # catches undefined behavior
#override CXXFLAGS += -Wold-style-cast #           # C-style cast in C++ code
#override CXXFLAGS += -Woverloaded-virtual #       # subclass shadowing makes parent impl's unavailable
#override CXXFLAGS += -Wsign-promo #               # gcc did what spec requires, but probably not what you want
#override CXXFLAGS += -Wstrict-null-sentinel #     # seems like a not-very-C++ thing to do? very unsure
#override CXXFLAGS += -Wsuggest-final-methods #    # does what it sounds like
#override CXXFLAGS += -Wsuggest-final-types #      # does what it sounds like
#override CXXFLAGS += -Wsuggest-override #         # overridden virtual func w/out override keyword
#override CXXFLAGS += -Wuseless-cast #             # pretty much what ya think here
#override CXXFLAGS += -Wzero-as-null-pointer-constant # use of 0 as NULL
#
## -Wc++-compat #                         # C/C++ compat issues
## -Wc++11-compat #                       # C11 compat issues
## -Wc++14-compat #                       # C14 compat issues
## -Wconditionally-supported #            # conditionally-supported (C++11 [intro.defs]) constructs (?)
## -Weffc++                               # violations of style guidelines from Meyers' Effective C++ books
## -Wmultiple-inheritance                 # used to enforce coding conventions, does what you'd think
## -Wnamespaces                           # used to enforce coding conventions, warn if namespace opened
## -Wnoexcept #                           # (?) I think warns if missing noexcept
## -Wnon-virtual-dtor #                   # something deeply c++, part of effc++
## -Wsynth                                # legacy flag, g++ != cfront
## -Wtemplates                            # used to enforce coding conventions, warn if new template
## -Wvirtual-inheritance                  # used to enforce coding conventions, does what you'd think
#
## Fortran-only warnings
## -Waliasing
## -Wampersand
## -Warray-temporaries
## -Wc-binding-type
## -Wcharacter-truncation
## -Wcompare-reals
## -Wconversion-extra
## -Wfunction-elimination
## -Wimplicit-interface
## -Wimplicit-procedure
## -Winteger-division
## -Wintrinsic-shadow
## -Wintrinsics-std
## -Wreal-q-constant
## -Wrealloc-lhs
## -Wrealloc-lhs-all
## -Wsurprising
## -Wtabs
## -Wtarget-lifetime
## -Wunused-dummy-argument
## -Wuse-without-only
#
## Objective-C(++)-only
## -Wassign-intercept
## -Wselector
## -Wstrict-selector-match
## -Wundeclared-selector
#
## END WARNINGS
###################################################################################################


#XXX: Update this with information we actually care about for NRF
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

endif

