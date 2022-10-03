
FROM ubuntu:focal AS development

RUN apt-get update && apt-get install -y make gcc git
WORKDIR /
RUN git clone --recursive --branch release-1.0 https://gitlab.com/neon-kit/shared-build-system.git
RUN git clone --recursive --branch release-1.0 https://gitlab.com/neon-kit/unit-testing-framework.git

FROM development as base
COPY /build /event-driven-system/build
COPY /source /event-driven-system/source
COPY /portable /event-driven-system/portable
COPY /tests /event-driven-system/tests
COPY /documentation /event-driven-system/documentation

FROM base AS build
WORKDIR /event-driven-system/build
RUN make test V=1

FROM scratch AS test_report
COPY --from=build /event-driven-system/generated/report_* /test_report/
