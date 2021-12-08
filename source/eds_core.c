
#include "eds_core.h"

#include <string.h>

extern inline void
eds_core__list_init(struct eds_object__list *self);

extern inline struct eds_object__list *
eds_core__list_next(struct eds_object__list *self);

extern inline struct eds_object__list *
eds_core__list_prev(struct eds_object__list *self);

extern inline void
eds_core__list_add_after(struct eds_object__list *self, struct eds_object__list *after);

extern inline void
eds_core__list_add_before(struct eds_object__list *self, struct eds_object__list *before);

extern inline void
eds_core__list_remove(struct eds_object__list *self);

extern inline bool
eds_core__list_is_empty(const struct eds_object__list *self);

void
eds_core__vector_init(struct eds_object__vector *self,
    void *entries,
    size_t item_size,
    size_t size)
{
    self->p__n_entries = 0u;
    self->p__n_size = size;
    self->p__item_size = item_size;
    self->p__entries = entries;
}

extern inline size_t
eds_core__vector_n_entries(const struct eds_object__vector * self);

extern inline size_t
eds_core__vector_n_size(const struct eds_object__vector * self);

extern inline bool
eds_core__vector_is_full(const struct eds_object__vector * self);

extern inline void *
eds_core__vector_peek(const struct eds_object__vector * self, uint32_t index);

void
eds_core__vector_insert(struct eds_object__vector *self, uint32_t index, const void * entry)
{
    void * s_item;

    s_item = (char *)self->p__entries + (self->p__item_size * index);
    if (index < self->p__n_entries) {
        void * d_item;
        size_t block_size;

        d_item = (char *)self->p__entries + (self->p__item_size * (index + 1u));
        block_size = self->p__item_size * (self->p__n_entries - index);
        memmove(d_item, s_item, block_size);
    }
    memcpy(s_item, entry, self->p__item_size);
    self->p__n_entries++;
}

void
eds_core__vector_remove(struct eds_object__vector *self, uint32_t index)
{
    if (index < (self->p__n_entries - 1u)) {
        void * d_item;
        void * s_item;
        size_t block_size;

        d_item = (char *)self->p__entries + (self->p__item_size * index);
        s_item = (char *)self->p__entries + (self->p__item_size * (index + 1u));
        block_size = self->p__item_size * (self->p__n_entries - index);
        memmove(d_item, s_item, block_size);
    }
    self->p__n_entries--;
}

void
eds_core__queue_init(struct eds_object__queue * self, uint32_t n_entries, void * storage)
{
    self->p__head = 0u;
    self->p__tail = 0u;
    self->p__n_entries = n_entries;
    self->p__n_free = n_entries;
    self->p__storage = storage;
}

void
eds_core__queue_term(struct eds_object__queue * self)
{
    self->p__head = 0u;
    self->p__tail = 0u;
    self->p__n_entries = 0u;
    self->p__n_free = 0u;
    self->p__storage = NULL;
}

void
eds_core__queue_put_fifo(struct eds_object__queue * self, void * item)
{
    self->p__storage[self->p__head++] = item;
    if (self->p__head == self->p__n_entries) {
        self->p__head = 0u;
    }
    self->p__n_free--;
}

void
eds_core__queue_put_lifo(struct eds_object__queue * self, void * item)
{
    self->p__tail--;
    if (self->p__tail >= self->p__n_entries) {
        self->p__tail = self->p__n_entries - 1u;
    }
    self->p__n_free--;
}

void *
eds_core__queue_get(struct eds_object__queue *self)
{
    void * entry;

    entry = self->p__storage[self->p__tail++];
    if (self->p__tail == self->p__n_entries) {
        self->p__tail = 0u;
    }
    self->p__n_free++;

    return entry;
}

void *
eds_core__queue_head(const struct eds_object__queue * self)
{
    uint32_t real_head;

	real_head = self->p__head;
    real_head--;
    if (real_head >= self->p__n_entries) {
        real_head = self->p__n_entries - 1u;
    }
    return self->p__storage[real_head];
}

void *
eds_core__queue_tail(const struct eds_object__queue * self)
{
    return self->p__storage[self->p__tail];
}

extern inline uint32_t
eds_core__queue_n_entries(const struct eds_object__queue * self);

extern inline uint32_t
eds_core__queue_n_free(const struct eds_object__queue * self);

extern inline bool
eds_core__queue_is_empty(const struct eds_object__queue * self);

extern inline bool
eds_core__queue_is_full(const struct eds_object__queue *self);