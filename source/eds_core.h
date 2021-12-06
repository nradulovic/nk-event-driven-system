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


/** @brief      Determines the first dimension of an array.
 *  @param      array
 *              An array : type unspecified
 *  @mseffect
 */
#define EDS_CORE__ARRAY_SIZE(array)         (sizeof(array) / sizeof(array[0]))

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
eds_core__escheduler__init(struct eds_object__escheduler *escheduler);

void
eds_core__escheduler__node_init(struct eds_object__escheduler_node *node, uint_fast8_t prio);

void
eds_core__escheduler__ready(struct eds_object__escheduler *escheduler,
    struct eds_object__escheduler_node *node);

void
eds_core__escheduler__block(struct eds_object__escheduler *escheduler,
    struct eds_object__escheduler_node *node);


#define EDS_CORE__VECTOR__INITIALIZER(entries)                                                  \
    {                                                                                           \
        .p__entries = (entries),                                                                \
        .p__item_size = sizeof((entries)[0]),                                                   \
        .p__length = 0u,                                                                        \
        .p__size = EDS_CORE__ARRAY_SIZE(entries),                                               \
    }

void
eds_core__vector__init(struct eds_object__vector *vector,
    void *entries,
    size_t item_size,
    size_t size);

inline size_t
eds_core__vector__length(const struct eds_object__vector * vector)
{
    return vector->p__length;
}

inline size_t
eds_core__vector__size(const struct eds_object__vector * vector)
{
    return vector->p__size;
}

inline bool
eds_core__vector__is_full(const struct eds_object__vector * vector)
{
    return (vector->p__length == vector->p__size);
}

inline void *
eds_core__vector__peek(const struct eds_object__vector * vector, uint32_t index)
{
    return (char *)vector->p__entries + (vector->p__item_size * index);
}

void
eds_core__vector__insert(struct eds_object__vector *vector, uint32_t index, const void * entry);

void
eds_core__vector__remove(struct eds_object__vector *vector, uint32_t index);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_CORE_H_ */
