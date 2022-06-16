
WS = ..
GIT_BASE_URL ?= git@gitlab.com:neon-kit
GIT_UTF_RELEASE ?= release-1.0
GIT_URL ?= $(GIT_BASE_URL)/unit-testing-framework.git

include $(WS)/build/sbs/definitions.mk

.PHONY: all
all: build_test_eds_core build_test_eds_mem

.PHONY: build_test_eds_core
build_test_eds_core: prep_unit_testing_framework
	$(VERBOSE)$(MAKE) -f test_eds_core.Makefile

.PHONY: build_test_eds_mem
build_test_eds_mem: prep_unit_testing_framework
	$(VERBOSE)$(MAKE) -f test_eds_mem.Makefile

.PHONY: clean
clean: clean_test_eds_core clean_test_eds_mem clean_docs

.PHONY: clean_test_eds_core
clean_test_eds_core: prep_unit_testing_framework
	$(VERBOSE)$(MAKE) -f test_eds_core.Makefile clean

.PHONY: clean_test_eds_mem
clean_test_eds_mem: prep_unit_testing_framework
	$(VERBOSE)$(MAKE) -f test_eds_mem.Makefile clean

.PHONY: test
test: test_eds_core test_eds_mem
	$(PRINT) All tests:
	$(PRINT) OK

.PHONY: test_eds_core
test_eds_core: prep_unit_testing_framework
	$(VERBOSE)$(MAKE) -f test_eds_core.Makefile test

.PHONY: test_eds_mem
test_eds_mem: prep_unit_testing_framework
	$(VERBOSE)$(MAKE) -f test_eds_mem.Makefile test

prep_unit_testing_framework: $(WS)/$(DEF_EXTERNAL_DIR)/unit-testing-framework
	$(PRINT) Unit testing framework: $(GIT_UTF_RELEASE)

$(WS)/$(DEF_EXTERNAL_DIR)/unit-testing-framework:
	$(VERBOSE)cd $(WS)/$(DEF_EXTERNAL_DIR) && git clone --branch $(GIT_UTF_RELEASE) $(GIT_URL)

.PHONY: docs
docs:
	doxygen

.PHONY: clean_docs
clean_docs:
	$(VERBOSE)$(RM) -rf $(DEF_DOX_O_DIR)/html

