# Various helper functions and definitions for use by nrf makefiles. Included
# by AppMakefile.mk

# ensure that this file is only included once
ifndef HELPERS_MAKEFILE
HELPERS_MAKEFILE = 1

# http://stackoverflow.com/questions/10858261/abort-makefile-if-variable-not-set
# Check that given variables are set and all have non-empty values,
# die with an error otherwise.
#
# Params:
#   1. Variable name(s) to test.
#   2. (optional) Error message to print.
check_defined = \
    $(strip $(foreach 1,$1, \
        $(call __check_defined,$1,$(strip $(value 2)))))
__check_defined = \
    $(if $(value $1),, \
      $(error Undefined $1$(if $2, ($2))))

# Check for a ~/ at the beginning of a path variable (NRF_BASE_DIR).
# Make will not properly expand this.
ifdef NRF_BASE_DIR
    ifneq (,$(findstring BEGINNINGOFVARIABLE~/,BEGINNINGOFVARIABLE$(NRF_BASE_DIR)))
        $(error Hi! Using "~" in Makefile variables is not supported. Use "$$(HOME)" instead)
    endif
endif

#XXX: add back in the gcc minimum version test for 6

#XXX: probably move this all out of Helpers. It's confusing right now
#########################################################################################
## Pretty-printing rules

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

endif

