# Commands and configurations for loading code onto hardware with JTAG
# Included by AppMakefile.mk

# ensure that this file is only included once
ifndef JTAG_MAKEFILE
JTAG_MAKEFILE = 1


# ---- JTAG configurations

# JTAG tools
JLINK = JLinkExe
JLINK_GDBSERVER = JLinkGDBServer
JLINK_RTTCLIENT = JLinkRTTClient

# Default port for GDB
GDB_PORT_NUMBER ?= 2331

# Configuration flags for JTAG tools
JLINK_FLAGS = -device $(FULL_IC) -if swd -speed 1000
JLINK_GDBSERVER_FLAGS = -port $(GDB_PORT_NUMBER)

# Allow users to select a specific JTAG device with a variable
ifdef SEGGER_SERIAL
  JLINK_FLAGS += -SelectEmuBySn $(SEGGER_SERIAL)
  JLINK_GDBSERVER_FLAGS += -select USB=$(SEGGER_SERIAL)
endif

# Configuration for test_softdevice command
# Download 16 bytes including the softdevice fimrware ID to disambiguate softdevices
# 16 bytes should be enough to distinguish from previously loaded non-softdevice code too
# https://devzone.nordicsemi.com/f/nordic-q-a/1171/how-do-i-access-softdevice-version-string
SOFTDEVICE_TEST_ADDR = 0x3000
SOFTDEVICE_TEST_LEN = 0x10

TERMINAL ?= x-terminal-emulator


# ---- JTAG rules

.PHONY: flash
flash: all test_softdevice
	$(Q)printf "r\n" > $(BUILDDIR)flash.jlink
ifdef ID
	$(Q)printf "w4 $(ID_FLASH_LOCATION), 0x$(ID_SECON) 0x$(ID_FIRST)\n" >> $(BUILDDIR)flash.jlink
endif
	$(Q)printf "loadfile $(HEX) \nr\ng\nexit\n" >> $(BUILDDIR)flash.jlink
	$(Q)$(JLINK) $(JLINK_FLAGS) $(BUILDDIR)flash.jlink

.PHONY: test_softdevice
test_softdevice: $(BUILDDIR)
ifneq ($(SOFTDEVICE_MODEL),blank)
	$(Q)printf "r\nsavebin $(BUILDDIR)downloaded_test.bin $(SOFTDEVICE_TEST_ADDR) $(SOFTDEVICE_TEST_LEN)\nexit\n" > $(BUILDDIR)test-softdevice.jlink
	$(Q)touch $(BUILDDIR)downloaded_test.bin
	$(Q)$(JLINK) $(JLINK_FLAGS) $(BUILDDIR)test-softdevice.jlink
	$(Q)$(OBJCOPY) -Iihex -Obinary $(SOFTDEVICE_PATH) $(BUILDDIR)softdevice_bin.bin
	$(Q)dd skip=$(shell printf "%d" $(SOFTDEVICE_TEST_ADDR)) count=$(shell printf "%d" $(SOFTDEVICE_TEST_LEN)) if=$(BUILDDIR)/softdevice_bin.bin of=$(BUILDDIR)softdevice_test.bin bs=1
	$(Q)rm -f $(BUILDDIR)/softdevice_bin.bin
	$(Q)diff -q $(BUILDDIR)downloaded_test.bin $(BUILDDIR)softdevice_test.bin || $(MAKE) flash_softdevice
endif

.PHONY: flash_debug
flash_debug: debug test_softdevice
	$(Q)printf "r\n" > $(BUILDDIR)debug-flash.jlink
ifdef ID
	$(Q)printf "w4 $(ID_FLASH_LOCATION), 0x$(ID_SECON) 0x$(ID_FIRST)\n" >> $(BUILDDIR)debug-flash.jlink
endif
	$(Q)printf "loadfile $(DEBUG_HEX) \nr\ng\nexit\n" >> $(BUILDDIR)debug-flash.jlink
	$(Q)$(JLINK) $(JLINK_FLAGS) $(BUILDDIR)debug-flash.jlink

.PHONY: flash_softdevice
flash_softdevice: $(BUILDDIR) $(SOFTDEVICE_PATH)
	$(Q)printf "w4 4001e504 1\nloadfile $(SOFTDEVICE_PATH) \nr\ng\nexit\n" > $(BUILDDIR)flash_softdevice.jlink
	$(Q)$(JLINK) $(JLINK_FLAGS) $(BUILDDIR)flash_softdevice.jlink

.PHONY: erase
erase: $(BUILDDIR)
	$(Q)printf "w4 4001e504 2\nw4 4001e50c 1\nsleep 100\nr\nexit\n" > $(BUILDDIR)erase.jlink
	$(Q)$(JLINK) $(JLINK_FLAGS) $(BUILDDIR)erase.jlink

.PHONY: gdb
gdb:
	$(Q)$(TERMINAL) -e "$(JLINK_GDBSERVER) $(JLINK_FLAGS) $(JLINK_GDBSERVER_FLAGS)"
	$(Q)sleep 1
	$(Q)printf "target remote localhost:$(GDB_PORT_NUMBER)\nload\nmon reset\nbreak main\n" > .gdbinit
ifneq ("$(wildcard $(DEBUG_ELF))","")
	$(Q)$(TERMINAL) -e "$(GDB) $(DEBUG_ELF)"
else ifneq ("$(wildcard $(ELF))","")
	$(Q)$(TERMINAL) -e "$(GDB) $(ELF)"
else
	$(Q)$(TERMINAL) -e "$(GDB)"
endif

.PHONY: rtt
rtt:
	$(Q)$(TERMINAL) -e "$(JLINK) $(JLINK_FLAGS) -AutoConnect 1"
	$(Q)sleep 1
	$(Q)$(TERMINAL) -e "$(JLINK_RTTCLIENT)"

.PHONY: clean
clean::
	$(Q)rm -rf JLink.log .gdbinit

endif

