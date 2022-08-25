
WS = ..

include $(WS)/build/../external/sbs/definitions.mk

.PHONY: all
all: build_test_eds_core build_test_eds_mem build_test_no_allocations

.PHONY: build_test_eds_core
build_test_eds_core: 
	$(VERBOSE)$(MAKE) -f test_eds_core.Makefile

.PHONY: build_test_eds_mem
build_test_eds_mem: 
	$(VERBOSE)$(MAKE) -f test_eds_mem.Makefile

.PHONY: build_test_no_allocations
build_test_no_allocations: 
	$(VERBOSE)$(MAKE) -f test_no_allocations.Makefile

.PHONY: clean
clean: clean_test_eds_core clean_test_eds_mem clean_docs clean_test_no_allocations

.PHONY: clean_test_eds_core
clean_test_eds_core: 
	$(VERBOSE)$(MAKE) -f test_eds_core.Makefile clean

.PHONY: clean_test_eds_mem
clean_test_eds_mem: 
	$(VERBOSE)$(MAKE) -f test_eds_mem.Makefile clean

.PHONY: clean_test_no_allocations
clean_test_no_allocations: 
	$(VERBOSE)$(MAKE) -f test_no_allocations.Makefile clean

.PHONY: test
test: test_eds_core test_eds_mem test_no_allocations
	$(PRINT) All tests:
	$(PRINT) OK
.PHONY: test_report
test_report: test
	@for report in $(WS)/$(DEF_GENERATED_DIR)/report_test_*; do echo $$report:; cat $$report; done

.PHONY: test_eds_core
test_eds_core: 
	$(VERBOSE)$(MAKE) -f test_eds_core.Makefile test

.PHONY: test_eds_mem
test_eds_mem: 
	$(VERBOSE)$(MAKE) -f test_eds_mem.Makefile test

.PHONY: test_no_allocations
test_no_allocations: 
	$(VERBOSE)$(MAKE) -f test_no_allocations.Makefile test

.PHONY: docs
docs:
	doxygen

.PHONY: clean_docs
clean_docs:
	$(VERBOSE)$(RM) -rf $(DEF_DOX_O_DIR)/html

