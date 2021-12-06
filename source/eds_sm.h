/*
 * eds_sm.h
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_EDS_SM_H_
#define NEON_KIT_GENERIC_SOURCE_EDS_SM_H_

#include "eds_object.h"

void
eds_sm__init(struct eds_object__sm *sm, nk_eds_sm__state_fn *initial_state, void *workspace);

nk_eds_sm__action
eds_sm__dispatch(struct eds_object__sm *sm, const struct eds_object__event *event);

inline nk_eds_sm__action
eds_sm__action_handled(struct eds_object__sm *sm)
{
    return EDS_SM__ACTION__HANDLED;
}

inline nk_eds_sm__action
eds_sm__action_transit(struct eds_object__sm *sm, nk_eds_sm__state_fn * next_state)
{
    sm->p__current = next_state;
    return EDS_SM__ACTION__TRANSIT;
}

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_SM_H_ */
