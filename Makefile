
include build/sbs/definitions.mk

.PHONY: test
test:
	$(VERBOSE)docker build -t unit-tests -f Dockerfile .
	$(VERBOSE)docker create -ti --name nk_dummy unit-tests:latest bash
	$(VERBOSE)docker cp nk_dummy:/test_report ./generated/
	$(VERBOSE)docker rm -f nk_dummy
	@for report in ./generated/report*; do echo Report: $$report; cat $$report; done
