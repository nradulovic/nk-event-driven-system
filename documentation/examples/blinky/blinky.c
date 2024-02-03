
#include "blinky.h"

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define EVENT_TICK      1000

static void log_message(const char * format, ...)
{
    va_list ap;
    clock_t clock_ticks;

    clock_ticks = clock();
    printf("[%llu] ", (long long unsigned)clock_ticks);
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}

static eds__sm_action
sm__blink_off(eds__sm * sm, void * workspace, const eds__event * event);
static eds__sm_action
sm__blink_on(eds__sm * sm, void * workspace, const eds__event * event);

static eds__sm_action
sm__blink_off(eds__sm * sm, void * workspace, const eds__event * event)
{
    (void)workspace;

    switch (eds__event_id(event)) {
    case EDS__SM_EVENT__INIT:
        log_message("blinky SM: blink_off: init\n");
        return eds__sm_event_handled(sm);
    case EVENT_TICK:
        log_message("blinky SM: blink_off: event_tick\n");
        return eds__sm_transit_to(sm, sm__blink_on);
    default:
        return eds__sm_event_ignored(sm);
    }
}

static eds__sm_action
sm__blink_on(eds__sm * sm, void * workspace, const eds__event * event)
{
    (void)workspace;

    switch (eds__event_id(event)) {
    case EDS__SM_EVENT__INIT:
        log_message("blinky SM: blink_on: init\n");
        return eds__sm_event_handled(sm);
    case EVENT_TICK:
        log_message("blinky SM: blink_on: event_tick\n");
        return eds__sm_transit_to(sm, sm__blink_off);
    default:
        return eds__sm_event_ignored(sm);
    }
}

eds__sm_action
blinky_initial_state(eds__sm * sm, void * workspace, const eds__event * event)
{
    struct blinky_workspace * ws = workspace;
    eds__error error;

    switch (eds__event_id(event)) {
    case EDS__SM_EVENT__INIT:
        log_message("blinky SM: init: init\n");
        error = eds__etimer_create(sm, EVENT_TICK, NULL, &ws->tick);
        assert(error == EDS__ERROR_NONE);
        error = eds__etimer_send_every(ws->tick, 1000);
        assert(error == EDS__ERROR_NONE);
        return eds__sm_transit_to(sm, sm__blink_on);
    default:
        return eds__sm_event_ignored(sm);
    }
}
