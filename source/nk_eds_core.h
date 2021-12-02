/*
 * eds_core.h
 *
 *  Created on: Dec 2, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_NK_EDS_CORE_H_
#define NEON_KIT_GENERIC_SOURCE_NK_EDS_CORE_H_

#include <stdbool.h>

#include "nk_eds_object.h"

struct eds_object__list;

void eds_list__init(struct eds_object__list * self);
struct eds_object__list*
eds_list__next(struct eds_object__list * self);
struct eds_object__list*
eds_list__prev(struct eds_object__list * self);
void eds_list__add_after(struct eds_object__list * self, struct eds_object__list * after);
void eds_list__add_before(struct eds_object__list * self, struct eds_object__list * after);
void eds_list__remove(struct eds_object__list * self);
bool eds_list__is_empty(const struct eds_object__list * self);

void eds_sm_executor__init(struct eds_object__sm_executor * sm_executor, eds_sm__state_fn * initial_state, void * workspace);
eds_sm__action eds_sm_executor__dispatch(struct eds_object__sm_executor * sm_executor, const eds_object__event * event);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_CORE_H_ */
