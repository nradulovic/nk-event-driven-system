/*
 * nk_tasker.h
 *
 *  Created on: May 8, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 */

#ifndef NEON_KIT_GENERIC_NK_TASKER_H_
#define NEON_KIT_GENERIC_NK_TASKER_H_

#include <stdint.h>
#include "generic/composite/nk_farray.h"
#include "generic/composite/nk_array.h"
#include "generic/composite/nk_list.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#define NK_ENABLED_TASKER

/**
 * @brief   Maximum priority number
 */
#define NK_TASKER__MAX_PRIO             (31u)

/**
 * @brief   Forward declaration of task structure
 */
struct nk_task;


/**
 * @brief   Tasker structure
 */
struct nk_tasker
{
    struct nk_dqueue p__ready_tasks;
    struct nk_task *p__current;
};

/** @brief  Initialize tasker instances
 *
 * Initialize each instance given in instance array
 */
void
nk_tasker__initialize(struct nk_tasker *self);

void
nk_tasker__start(struct nk_tasker *self,
                 struct nk_task *task);

void
nk_tasker__yield(struct nk_tasker *self);

void
nk_tasker__disable(struct nk_tasker *self);

void
nk_tasker__enable(struct nk_tasker *self);

static inline struct nk_task*
nk_tasker__current(struct nk_tasker *self)
{
    return self->p__current;
}

#define NK_TASK__INITIALIZER(self, method, arg, prio)                \
        {                                                               \
            .p__entry = NK_LIST__INITIALIZER(&(self).p__entry, &(self)),\
            .p__arg = (arg),                                            \
            .p__method = (method),                                      \
            .p__prio = (prio),                                          \
        },                                                              \

typedef void
(nk_task__method_t)(void*);

struct nk_task
{
    struct nk_list p__entry;
    void *p__arg;
    nk_task__method_t *p__method;
    uint_fast32_t p__prio;
};

void
nk_task__initialize(struct nk_task *self);

static inline uint32_t
nk_task__get_prio(const struct nk_task *self)
{
    return (uint32_t) self->p__prio;
}

#if defined(__cplusplus)
}
#endif

#endif /* NEON_KIT_GENERIC_NK_TASKER_H_ */
