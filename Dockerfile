
FROM ubuntu:focal AS development

RUN apt-get update && apt-get install -y make gcc
COPY /build /nk/build
COPY /source /nk/source
COPY /portable /nk/portable
COPY /tests /nk/tests
COPY /documentation /nk/documentation

FROM development AS build
WORKDIR /nk/build
RUN make test

FROM scratch AS test_report
COPY --from=build /nk/generated/report_* /test_report/
