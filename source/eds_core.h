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
eds_core__list__init(struct eds_object__list *self)
{
    self->p__next = self;
    self->p__prev = self;
}

inline struct eds_object__list*
eds_core__list__next(struct eds_object__list *self)
{
    return self->p__next;
}

inline struct eds_object__list*
eds_core__list__prev(struct eds_object__list *self)
{
    return self->p__prev;
}

inline void
eds_core__list__add_after(struct eds_object__list *self, struct eds_object__list *after)
{
    self->p__next = after->p__next;
    self->p__prev = after;
    after->p__next->p__prev = self;
    after->p__next = self;
}

inline void
eds_core__list__add_before(struct eds_object__list *self, struct eds_object__list *before)
{
    self->p__next = before;
    self->p__prev = before->p__prev;
    before->p__prev->p__next = self;
    before->p__prev = self;
}

inline void
eds_core__list__remove(struct eds_object__list *self)
{
    self->p__next->p__prev = self->p__prev;
    self->p__prev->p__next = self->p__next;
}

inline bool
eds_core__list__is_empty(const struct eds_object__list *self)
{
    return !!(self->p__next == self);
}

void
eds_core__mem__init(void);

void
eds_core__mem__add(struct eds_object__mem *mem);

struct eds_object__mem*
eds_core__mem__select(size_t size);

void*
eds_core__mem__allocate(struct eds_object__mem *mem, size_t size);

void
eds_core__mem__deallocate(struct eds_object__mem *mem, void *block);

inline size_t
eds_core__equeue__calculate_storage_size(size_t entries)
{
    return sizeof(struct eds_object__event*) * entries;
}

void
eds_core__equeue__init(struct eds_object__equeue *equeue,
    size_t entries,
    struct eds_object__event *storage);

void
eds_core__equeue__term(struct eds_object__equeue *equeue);

void
eds_core__equeue__push(struct eds_object__equeue *equeue, const struct eds_object__event *event);

const struct eds_object__event*
eds_core__equeue__pop(struct eds_object__equeue *equeue);

bool
eds_core__equeue__is_empty(const struct eds_object__equeue *equeue);

void
eds_core__sm_executor__init(struct eds_object__sm_executor *sm_executor,
    eds_sm__state_fn *initial_state,
    void *workspace);

nk_eds_sm__action
eds_core__sm_executor__dispatch(struct eds_object__sm_executor *sm_executor,
    const struct eds_object__event *event);

void
eds_core__escheduler__node_init(struct eds_object__escheduler_node *node, uint_fast8_t prio);

void
eds_core__escheduler__block(struct eds_object__escheduler *escheduler,
    struct eds_object__escheduler_node *node);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_CORE_H_ */
