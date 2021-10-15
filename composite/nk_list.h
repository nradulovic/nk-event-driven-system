/*
 * nk_list.h
 *
 *  Created on: Apr 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_NK_LIST_H_
#define NEON_KIT_GENERIC_NK_LIST_H_

#include <stdbool.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define NK_LIST__INITIALIZER(list, object)        \
        {                                           \
            .p__next = (list),                      \
            .p__prev = (list),                      \
        }

struct nk_list
{
    struct nk_list *p__next;
    struct nk_list *p__prev;
};

static inline void
nk_list__init(struct nk_list *self)
{
    self->p__next = self;
    self->p__prev = self;
}

static inline struct nk_list*
nk_list__next(struct nk_list *self)
{
    return self->p__next;
}

/**
 *
 * (M) next-> (other) next-> (N)
 * (M) next-> (other) next-> (self) next-> (N)
 */
static inline void
nk_list__add_after(struct nk_list *self,
                   struct nk_list *other)
{
    self->p__next = other->p__next;
    self->p__prev = other;
    self->p__next->p__prev = self;
    self->p__prev->p__next = self;
}

/**
 *
 * (M) next-> (other) next-> (N)
 * (M) next-> (self)  next-> (other) next-> (N)
 */
static inline void
nk_list__add_before(struct nk_list *self,
                    struct nk_list *other)
{
    self->p__next = other;
    self->p__prev = other->p__prev;
    self->p__next->p__prev = self;
    self->p__prev->p__next = self;
}

static inline void
nk_list__add_sorted(struct nk_list *self,
                    struct nk_list *list,
                    bool
                    (*discriminator)(const struct nk_list *lhs,
                                     const struct nk_list *rhs))
{
    struct nk_list *current;

    current = nk_list__next(list);
    for (; current != list; current = nk_list__next(current)) {
        if (discriminator(self, current)) {
            break;
        }
    }
    nk_list__add_after(self, current);
}

/**
 * (M) next-> (self) next-> (N)
 * (M) next-> (N)
 */
static inline void
nk_list__remove(struct nk_list * self)
{
    self->p__next->p__prev = self->p__prev;
    self->p__prev->p__next = self->p__next;
}

bool nk_list__is_empty(const struct nk_list * self)
{
    return !!(self->p__next == self);
}
#if defined(__cplusplus)
}
#endif

#endif /* NEON_KIT_GENERIC_NK_LIST_H_ */
