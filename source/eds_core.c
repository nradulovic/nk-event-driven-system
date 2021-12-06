
#include "eds_core.h"

#include <string.h>

extern inline void
eds_core__list__init(struct eds_object__list *self);

extern inline struct eds_object__list *
eds_core__list__next(struct eds_object__list *self);

extern inline struct eds_object__list *
eds_core__list__prev(struct eds_object__list *self);

extern inline void
eds_core__list__add_after(struct eds_object__list *self, struct eds_object__list *after);

extern inline void
eds_core__list__add_before(struct eds_object__list *self, struct eds_object__list *before);

extern inline void
eds_core__list__remove(struct eds_object__list *self);

extern inline bool
eds_core__list__is_empty(const struct eds_object__list *self);

void
eds_core__vector__init(struct eds_object__vector *vector,
    void *entries,
    size_t item_size,
    size_t size)
{
    vector->p__length = 0u;
    vector->p__size = size;
    vector->p__item_size = item_size;
    vector->p__entries = entries;
}

extern inline size_t
eds_core__vector__length(const struct eds_object__vector * vector);

extern inline size_t
eds_core__vector__size(const struct eds_object__vector * vector);

extern inline bool
eds_core__vector__is_full(const struct eds_object__vector * vector);

extern inline void *
eds_core__vector__peek(const struct eds_object__vector * vector, uint32_t index);

void
eds_core__vector__insert(struct eds_object__vector *vector, uint32_t index, const void * entry)
{
    void * s_item;

    s_item = (char *)vector->p__entries + (vector->p__item_size * index);
    if (index < vector->p__length) {
        void * d_item;
        size_t block_size;

        d_item = (char *)vector->p__entries + (vector->p__item_size * (index + 1u));
        block_size = vector->p__item_size * (vector->p__length - index);
        memmove(d_item, s_item, block_size);
    }
    memcpy(s_item, entry, vector->p__item_size);
    vector->p__length++;
}

void
eds_core__vector__remove(struct eds_object__vector *vector, uint32_t index)
{
    if (index < (vector->p__length - 1u)) {
        void * d_item;
        void * s_item;
        size_t block_size;

        d_item = (char *)vector->p__entries + (vector->p__item_size * index);
        s_item = (char *)vector->p__entries + (vector->p__item_size * (index + 1u));
        block_size = vector->p__item_size * (vector->p__length - index);
        memmove(d_item, s_item, block_size);
    }
    vector->p__length--;
}