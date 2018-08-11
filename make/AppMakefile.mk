# application master makefile. Included by application makefiles

# The first target Make finds is its default. So this line needs to be first to
# specify `all` as our default rule
all:

# directory for built output
BUILDDIR ?= _build/

# ---- Include additional supporting makefiles

# Build settings
include $(NRF_BASE_DIR)/make/Configuration.mk

# Various file inclusions
include $(NRF_BASE_DIR)/make/Includes.mk

# Helper functions
include $(NRF_BASE_DIR)/make/Helpers.mk


#XXX: need to include code for flashing

# ---- Rules for building apps
.PHONY:	all
all:	$(OBJS) $(OBJS_AS) $(HEX)

$(BUILDDIR):
	$(TRACE_DIR)
	$(Q)mkdir -p $@

$(BUILDDIR)%.o: %.c | $(BUILDDIR)
	$(TRACE_CC)
	$(Q)$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@

$(BUILDDIR)%.s: %.S | $(BUILDDIR)
	$(TRACE_CC)
	$(Q)$(CC) -E $< > $@

$(BUILDDIR)%.os: $(BUILDDIR)%.s | $(BUILDDIR)
	$(TRACE_AS)
	$(Q)$(AS) $< -o $@

$(HEX): $(OBJS) $(OBJS_AS) $(LIBS) | $(BUILDDIR)
	$(TRACE_LD)
	$(Q)$(LD) $(LDFLAGS) $(OBJS_AS) $(OBJS) $(LIBS) -o $(ELF)
	$(TRACE_HEX)
	$(Q)$(OBJCOPY) -Oihex $(ELF) $(HEX)
	$(TRACE_BIN)
	$(Q)$(OBJCOPY) -Obinary $(ELF) $(BIN)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $(ELF)

.PHONY: lst
lst:	$(ELF)
	$(TRACE_LST)
	$(Q)$(OBJDUMP) $(OBJDUMP_FLAGS) $(ELF) > $(LST)

.PHONY: size
size:	$(ELF)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $^

.PHONY:
clean::
	rm -rf $(BUILDDIR)


#########################################################################################
# Include dependency rules for picking up header changes (by convention at bottom of makefile)
-include $(DEPS)
