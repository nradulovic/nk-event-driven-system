
#include "sys/eds_smp.h"
#include "sys/eds_evt.h"
#include "sys/eds_core.h"
#include "eds_port.h"

const struct eds_object__evt g__smp_events[] =
{
    [EDS__SM_EVENT__INIT] = EDS_EVT__INITIALIZER(EDS__SM_EVENT__INIT, 0),
    [EDS__SM_EVENT__ENTER] = EDS_EVT__INITIALIZER(EDS__SM_EVENT__ENTER, 0),
    [EDS__SM_EVENT__EXIT] = EDS_EVT__INITIALIZER(EDS__SM_EVENT__EXIT, 0),
    [EDS__SM_EVENT__SUPER] = EDS_EVT__INITIALIZER(EDS__SM_EVENT__SUPER, 0),
};

void
eds_smp__init(struct eds_object__smp * sm, eds_object__smp_state * initial_state, void * workspace)
{
    sm->p__state = initial_state;
    sm->p__workspace = workspace;
}

#define SM__ACTION__HANDLED_OR_IGNORED(action)                                                  \
    ((action) < EDS__SM__ACTION__SUPER)

eds_core__error
eds_smp__dispatch(struct eds_object__smp *sm, const struct eds_object__evt *event)
{
    eds__sm_action action;
    eds__sm_state * current_state;

    current_state = sm->p__state;

    action = current_state(sm, sm->p__workspace, event);

    while (action == EDS__SM__ACTION__TRANSIT) {
        action = current_state(sm, sm->p__workspace, &g__smp_events[EDS__SM_EVENT__EXIT]);
        if (!SM__ACTION__HANDLED_OR_IGNORED(action)) {
            return EDS_CORE__ERROR__BAD_STATE;
        }
        current_state = sm->p__state;
        action = current_state(sm, sm->p__workspace, &g__smp_events[EDS__SM_EVENT__ENTER]);
        if (!SM__ACTION__HANDLED_OR_IGNORED(action)) {
            return EDS_CORE__ERROR__BAD_STATE;
        }
        action = current_state(sm, sm->p__workspace, &g__smp_events[EDS__SM_EVENT__INIT]);
    }
    if (!SM__ACTION__HANDLED_OR_IGNORED(action)) {
        return EDS_CORE__ERROR__BAD_STATE;
    }
    sm->p__state = current_state;
    return EDS_CORE__ERROR_NONE;
}

extern inline eds__sm_action
eds_smp__action_handled(struct eds_object__smp *sm);

extern inline eds__sm_action
eds_smp__action_ignored(struct eds_object__smp *sm);

extern inline eds__sm_action
eds_smp__action_transit(struct eds_object__smp *sm, eds_object__smp_state * next_state);
