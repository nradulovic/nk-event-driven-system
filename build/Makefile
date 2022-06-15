
GIT_BASE_URL ?= git@gitlab.com:neon-kit
GIT_UTF_RELEASE ?= release-1.0
GIT_URL ?= $(GIT_BASE_URL)/unit-testing-framework.git

.PHONY: all
all: build_test_eds_core build_test_eds_mem

.PHONY: build_test_eds_core
build_test_eds_core: prep_unit_testing_framework
	$(MAKE) -f Makefile.test_eds_core

.PHONY: build_test_eds_mem
build_test_eds_mem: prep_unit_testing_framework
	$(MAKE) -f Makefile.test_eds_mem

.PHONY: clean
clean: clean_test_eds_core clean_test_eds_mem clean_docs

.PHONY: clean_test_eds_core
clean_test_eds_core: prep_unit_testing_framework
	$(MAKE) -f Makefile.test_eds_core clean

.PHONY: clean_test_eds_mem
clean_test_eds_mem: prep_unit_testing_framework
	$(MAKE) -f Makefile.test_eds_mem clean

.PHONY: test
test: test_eds_core test_eds_mem

.PHONY: test_eds_core
test_eds_core: prep_unit_testing_framework
	$(MAKE) -f Makefile.test_eds_core test

.PHONY: test_eds_mem
test_eds_mem: prep_unit_testing_framework
	$(MAKE) -f Makefile.test_eds_mem test

prep_unit_testing_framework: ../external/unit-testing-framework
	@echo Unit testing framework: $(GIT_UTF_RELEASE)

../external/unit-testing-framework:
	cd ../external && git clone --branch $(GIT_UTF_RELEASE) $(GIT_URL)

.PHONY: docs
docs:
	doxygen

.PHONY: clean_docs
clean_docs:
	@$(RM) -rf ../generated/html

