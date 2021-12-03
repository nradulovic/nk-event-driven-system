/*
 * eds_core.h
 *
 *  Created on: Dec 2, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_NK_EDS_CORE_H_
#define NEON_KIT_GENERIC_SOURCE_NK_EDS_CORE_H_

#include <stdbool.h>

#include "eds_object.h"

#define EDS_CORE__N_OF_EVENT_MEM            32u

extern struct eds_object__mem eds_core__p__event_mem[EDS_CORE__N_OF_EVENT_MEM];

inline void
eds_core__list__init(struct eds_object__list * self)
{
    self->p__next = self;
    self->p__prev = self;
}

inline struct eds_object__list*
eds_core__list__next(struct eds_object__list * self)
{
    return self->p__next;
}

inline struct eds_object__list*
eds_core__list__prev(struct eds_object__list * self)
{
    return self->p__prev;
}

inline void
eds_core__list__add_after(struct eds_object__list * self,
                          struct eds_object__list * after)
{
    self->p__next = after->p__next;
    self->p__prev = after;
    after->p__next->p__prev = self;
    after->p__next = self;
}

inline void
eds_core__list__add_before(struct eds_object__list * self,
                           struct eds_object__list * before)
{
    self->p__next = before;
    self->p__prev = before->p__prev;
    before->p__prev->p__next = self;
    before->p__prev = self;
}

inline void
eds_core__list__remove(struct eds_object__list * self)
{
    self->p__next->p__prev = self->p__prev;
    self->p__prev->p__next = self->p__next;
}

inline bool
eds_core__list__is_empty(const struct eds_object__list * self)
{
    return !!(self->p__next == self);
}

void eds_core__mem__init(void);
void eds_core__mem__add(struct eds_object__mem * mem);
struct eds_object__mem * eds_core__mem__select(size_t size);
void * eds_core__mem__allocate(struct eds_object__mem * mem,
                             size_t size);
void eds_core__mem__deallocate(struct eds_object__mem * mem,
                               void * block);

void eds_core__sm_executor__init(struct eds_object__sm_executor * sm_executor,
                                 eds_sm__state_fn * initial_state,
                                 void * workspace);
eds_sm__action eds_core__sm_executor__dispatch(struct eds_object__sm_executor * sm_executor,
                                               const struct eds_object__event * event);

struct eds_object__event *
eds_core__event__create(uint32_t event_id,
                        size_t event_data_size,
                        struct eds_object__mem * mem);

void
eds_core__event__init(struct eds_object__event * event,
                      uint32_t event_id,
                      size_t event_data_size,
                      struct eds_object__mem * mem);

extern inline void
eds_core__event__ref_up(struct eds_object__event * event)
{
    event->p__ref_count++;
}

extern inline void
eds_core__event__ref_down(struct eds_object__event * event)
{
    event->p__ref_count--;
}

extern inline uint32_t
eds_core__event__id(const struct eds_object__event * event)
{
    return event->p__id;
}

extern inline size_t
eds_core__event__size(const struct eds_object__event * event)
{
    return event->p__size;
}

extern inline const struct eds_object__mem *
eds_core__event__mem(const struct eds_object__event * event)
{
    return event->p__mem;
}

extern inline bool
eds_core__event__is_in_use(const struct eds_object__event * event)
{
    return (event->p__ref_count != 0u);
}

extern inline void *
eds_core__event__data(struct eds_object__event * event)
{
    if (event->p__size != 0u) {
        return event + 1u;
    } else {
        return NULL;
    }
}

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_CORE_H_ */
