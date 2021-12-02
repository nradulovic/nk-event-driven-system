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

#define EDS_CORE__N_OF_EVENT_MEM            32u

extern struct eds_object__mem eds_core__p__event_mem[EDS_CORE__N_OF_EVENT_MEM];

void eds_core__list__init(struct eds_object__list * self);
struct eds_object__list*
eds_core__list__next(struct eds_object__list * self);
struct eds_object__list*
eds_core__list__prev(struct eds_object__list * self);
void eds_core__list__add_after(struct eds_object__list * self, struct eds_object__list * after);
void eds_core__list__add_before(struct eds_object__list * self, struct eds_object__list * after);
void eds_core__list__remove(struct eds_object__list * self);
bool eds_core__list__is_empty(const struct eds_object__list * self);

void eds_core__mem__init(void);
void eds_core__mem__add(struct eds_object__mem * mem);
struct eds_object__mem * eds_core__mem__find(size_t);
void eds_core__mem__allocate(struct eds_object__mem * mem, size_t size);
void eds_core__mem__deallocate(struct eds_object__mem * mem, void * block);

void eds_core__sm_executor__init(struct eds_object__sm_executor * sm_executor, eds_sm__state_fn * initial_state, void * workspace);
eds_sm__action eds_core__sm_executor__dispatch(struct eds_object__sm_executor * sm_executor, const eds_object__event * event);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_CORE_H_ */
