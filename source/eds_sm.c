
#include "eds_sm.h"
#include "eds_core.h"
#include "eds_event.h"
#include "eds_port.h"

void
eds_sm__init(struct eds_object__sm * sm, nk_eds_sm__state_fn * initial_state, void * workspace)
{
    sm->p__current = initial_state;
    sm->p__workspace = workspace;
}

