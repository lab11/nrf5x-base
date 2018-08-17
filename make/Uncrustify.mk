# Commands and configurations for code beautification with uncrustify
# Included by AppMakefile.mk

# Ensure that this file is only included once
ifndef UNCRUSTIFY_MAKEFILE
UNCRUSTIFY_MAKEFILE = 1

# C/C++ Linter configuration
UNCRUSTIFY := $(NRF_BASE_DIR)/tools/uncrustify/uncrustify.sh

# Files to format
FORMATTED_FILES := $(patsubst %.c,$(BUILDDIR)format/%.uncrustify,$(SOURCES))

$(BUILDDIR)format:
	@mkdir -p $@

.PHONY: fmt format
fmt format:: $(FORMATTED_FILES)

#	(if [ $< != *"/sdk/"* ]; then $(Q)$(UNCRUSTIFY) -f $< -o $@; fi)
#	$(Q)cmp -s $< $@ || (if [ "$$CI" = "true" ]; then diff -y $< $@; rm $@; exit 1; else cp $@ $<; fi)
#	$(Q)(if [ $< != *"/sdk/"* ]; then echo $<; fi)

$(BUILDDIR)format/%.uncrustify: %.c | _format_check_unstaged
	if [ -z "${<##*sdk*}" = "$<" ]; then echo ${<}; fi


#XXX: bring this back at some point
# Rules to help validate build configuration
#fmt format::
#	$(Q)$(NRF_BASE_DIR)/tools/check_override.sh

# Format check rule
.PHONY: _format_check_unstaged
_format_check_unstaged:
	$(Q)$(NRF_BASE_DIR)/tools/check_unstaged.sh

endif

