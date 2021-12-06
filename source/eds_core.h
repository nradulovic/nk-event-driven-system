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

#define EDS_CORE__ERROR__NONE               0
#define EDS_CORE__ERROR__NO_RESOURCE        0x1
#define EDS_CORE__ERROR__NO_MEMORY          0x2
#define EDS_CORE__ERROR__NO_PERMISSION      0x3
#define EDS_CORE__ERROR__IN_USE             0x4

typedef uint_fast8_t eds_core__error;

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

void
eds_core__mem__allocate(size_t data_size, struct eds_object__mem **mem, void **memory);

void*
eds_core__mem__allocate_from(struct eds_object__mem *mem, size_t size);

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
eds_core__escheduler__init(struct eds_object__escheduler *escheduler);

void
eds_core__escheduler__node_init(struct eds_object__escheduler_node *node, uint_fast8_t prio);

void
eds_core__escheduler__ready(struct eds_object__escheduler *escheduler,
    struct eds_object__escheduler_node *node);

void
eds_core__escheduler__block(struct eds_object__escheduler *escheduler,
    struct eds_object__escheduler_node *node);

void
eds_core__vector__init(struct eds_object__vector *vector,
    void *entries,
    uint32_t * keys,
    size_t item_size,
    size_t size);

inline size_t
eds_core__vector__length(const struct eds_object__vector * vector)
{
    return vector->p__length;
}

void
eds_core__vector__insert(struct eds_object__vector *vector, uint32_t index, const void * entry);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_CORE_H_ */
