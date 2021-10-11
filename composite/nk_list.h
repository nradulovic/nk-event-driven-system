/*
 * nk_list.h
 *
 *  Created on: Apr 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_NK_LIST_H_
#define NEON_KIT_GENERIC_NK_LIST_H_

#if defined(__cplusplus)
extern "C"
{
#endif

struct nk_list
{
    struct nk_list * p__next;
    struct nk_list * p__prev;
    void * p__object;
};

static inline void
nk_list__init(struct nk_list * self, void * object)
{
    self->p__next = self;
    self->p__prev = self;
    self->p__object = object;
}

static inline struct nk_list *
nk_list__next(struct nk_list * self)
{
    return self->p__next;
}

#if defined(__cplusplus)
}
#endif

#endif /* NEON_KIT_GENERIC_NK_LIST_H_ */
