
#include "blinky.h"
#include <stdio.h>

#define EVENT_TICK      1000

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
        printf("SM: blink_off: init\n");
        return eds__sm_event_handled(sm);
    case EVENT_TICK:
        printf("SM: blink_off: event_tick\n");
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
        printf("SM: blink_on: init\n");
        return eds__sm_event_handled(sm);
    case EVENT_TICK:
        printf("SM: blink_on: event_tick\n");
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
    eds__event * n_event;

    switch (eds__event_id(event)) {
    case EDS__SM_EVENT__INIT:
        printf("SM: init: init\n");
        error = eds__etimer_create(NULL, &ws->tick);
        assert(error == EDS__ERROR_NONE);
        error = eds__event_create(EVENT_TICK, 0, &n_event);
        assert(error == EDS__ERROR_NONE);
        error = eds__etimer_send_every(ws->tick, eds__agent_from_sm(sm), n_event, 1000);
        assert(error == EDS__ERROR_NONE);
        return eds__sm_transit_to(sm, sm__blink_on);
    default:
        return eds__sm_event_ignored(sm);
    }
}