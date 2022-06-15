
.PHONY: test
test:
	@docker build -t unit-tests -f Dockerfile .
	@docker create -ti --name nk_dummy unit-tests:latest bash
	@docker cp nk_dummy:/test_report ./generated/
	@docker rm -f nk_dummy
	@for report in ./generated/report*; do echo Report: $$report; cat $$report; done
