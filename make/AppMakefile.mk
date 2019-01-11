# application master makefile. Included by application makefiles

#XXX: explain the required variables here


# The first target Make finds is its default. So this line needs to be first to
# specify `all` as our default rule
all:


# ---- Output filenames

OUTPUT_NAME ?= $(addsuffix _sdk$(SDK_VERSION)_$(SOFTDEVICE_MODEL), $(PROJECT_NAME))
HEX = $(BUILDDIR)$(OUTPUT_NAME).hex
DEBUG_HEX = $(BUILDDIR)$(OUTPUT_NAME).hex-debug
FIRST_DFU_HEX = $(BUILDDIR)$(OUTPUT_NAME)_first_dfu.hex
ELF = $(BUILDDIR)$(OUTPUT_NAME).elf
DEBUG_ELF = $(BUILDDIR)$(OUTPUT_NAME).elf-debug
BIN = $(BUILDDIR)$(OUTPUT_NAME).bin
DEBUG_BIN = $(BUILDDIR)$(OUTPUT_NAME).bin-debug
LST = $(BUILDDIR)$(OUTPUT_NAME).lst
MAP = $(BUILDDIR)$(OUTPUT_NAME).Map


# ---- Include additional supporting makefiles

# Build settings
include $(NRF_BASE_DIR)/make/Configuration.mk

# Various file inclusions
include $(NRF_BASE_DIR)/make/Includes.mk

# JTAG flashing configuration and rules
include $(NRF_BASE_DIR)/make/Program.mk


# ---- Rules for building apps
.PHONY:	all
all: $(OBJS) $(OBJS_AS) $(HEX)

$(BUILDDIR):
	$(TRACE_DIR)
	$(Q)mkdir -p $@

$(BUILDDIR)%.o: %.c | $(BUILDDIR)
	$(TRACE_CC)
	$(Q)$(CC) $(LDFLAGS) $(CFLAGS) $(OPTIMIZATION_FLAG) $< -o $@

$(BUILDDIR)%.o-debug: %.c | $(BUILDDIR)
	$(TRACE_CC)
	$(Q)$(CC) $(LDFLAGS) $(CFLAGS) -g -O0 $< -o $@

.PRECIOUS: $(BUILDDIR)%.s
$(BUILDDIR)%.s: %.S | $(BUILDDIR)
	$(TRACE_CC)
	$(Q)$(CC) -E $< > $@

$(BUILDDIR)%.os: $(BUILDDIR)%.s | $(BUILDDIR)
	$(TRACE_AS)
	$(Q)$(AS) $< -o $@

$(BUILDDIR)%.os-debug: $(BUILDDIR)%.s | $(BUILDDIR)
	$(TRACE_AS)
	$(Q)$(AS) $< -o $@

$(ELF): $(OBJS) $(OBJS_AS) $(LIBS) | $(BUILDDIR)
	$(TRACE_LD)
	$(Q)$(LD) $(LDFLAGS) -Wl,--start-group $^ $(LDLIBS) -Wl,--end-group -o $@

$(DEBUG_ELF): $(DEBUG_OBJS) $(DEBUG_OBJS_AS) $(LIBS) | $(BUILDDIR)
	$(TRACE_LD)
	$(Q)$(LD) $(LDFLAGS) -Wl,--start-group $^ $(LDLIBS) -Wl,--end-group -o $@

$(HEX): $(ELF) | $(BUILDDIR)
	$(TRACE_HEX)
	$(Q)$(OBJCOPY) -Oihex $< $(HEX)
	$(TRACE_BIN)
	$(Q)$(OBJCOPY) -Obinary $< $(BIN)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

$(DEBUG_HEX): $(DEBUG_ELF) | $(BUILDDIR)
	$(TRACE_HEX)
	$(Q)$(OBJCOPY) -Oihex $< $(DEBUG_HEX)
	$(TRACE_BIN)
	$(Q)$(OBJCOPY) -Obinary $< $(DEBUG_BIN)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

.PHONY: debug
debug: $(DEBUG_OBJS) $(DEBUG_OBJS_AS) $(DEBUG_HEX)

.PHONY: lst
lst: $(ELF)
	$(TRACE_LST)
	$(Q)$(OBJDUMP) $(OBJDUMP_FLAGS) $< > $(LST)

.PHONY: size
size: $(ELF)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

.PHONY: clean
clean::
	@echo " Cleaning..."
	$(Q)rm -rf $(BUILDDIR)


# ---- Dependencies
# Include dependency rules for picking up header changes (by convention at bottom of makefile)
-include $(DEPS)
