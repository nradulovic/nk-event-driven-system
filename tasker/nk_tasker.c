/*
 * nk_tasker.c
 *
 *  Created on: May 8, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 */

#include "generic/tasker/nk_tasker.h"

#include <stddef.h>

#include "generic/portable/nk_cpu.h"
#include "generic/portable/nk_compiler.h"

static struct nk_tasker__node*
get_highest__from_tasker(const struct nk_tasker *tasker)
{
    return NK_COMPILER__CONTAINER_OF(tasker->p__sentinels.items[nk_cpu__ffs(tasker->p__bitmap)],
                                     struct nk_tasker__node,
                                     p__entry);

}

static bool
priority_discriminator(const struct nk_list *lhs,
                       const struct nk_list *rhs)
{
    const struct nk_tasker__node *lhs_task =
            NK_COMPILER__CONTAINER_OF_CONST(lhs,
                                            struct nk_tasker__node,
                                            p__entry);
    const struct nk_tasker__node *rhs_task =
            NK_COMPILER__CONTAINER_OF_CONST(rhs,
                                            struct nk_tasker__node,
                                            p__entry);

    if (lhs_task->p__prio < rhs_task->p__prio) {
        return true;
    } else {
        return false;
    }
}

void
nk_tasker__init(struct nk_tasker *tasker)
{
    tasker->p__bitmap = 0u;
    for (size_t i = 0u; i < NK_FARRAY__SIZE(&tasker->p__sentinels); i++) {
        tasker->p__sentinels.items[i] = NULL;
    }
}

void
nk_tasker__blocker__init(struct nk_tasker__blocker *blocker)
{
    nk_list__init(&blocker->p__sentinel);
}

void
nk_tasker__node__init(struct nk_tasker__node *node,
                      uint_fast8_t priority)
{
    nk_list__init(&node->p__entry);
    node->p__prio = priority;
}

/**
 * \brief       Put a sleeping node into the running state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__run(struct nk_tasker *tasker,
               struct nk_tasker__node *node)
{
    if (tasker->p__sentinels.items[node->p__prio] == NULL) {
        tasker->p__sentinels.items[node->p__prio] = &node->p__entry;
        tasker->p__bitmap |= 0x1u << node->p__prio;
    } else {
        nk_list__add_before(tasker->p__sentinels.items[node->p__prio],
                            &node->p__entry);
    }
    return get_highest__from_tasker(tasker);
}

/**
 * \brief       Put a running node into the sleeping state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__sleep(struct nk_tasker *tasker,
                 struct nk_tasker__node *node)
{
    nk_list__remove(&node->p__entry);
    if (nk_list__is_empty(&node->p__entry)) {
        tasker->p__bitmap &= ~(0x1u << node->p__prio);
        tasker->p__sentinels.items[node->p__prio] = NULL;
    }
    return get_highest__from_tasker(tasker);
}

/**
 * \brief       Put the highest priority running node into the blocked state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__block(struct nk_tasker *tasker,
                 struct nk_tasker__node *node,
                 struct nk_tasker__blocker *blocker)
{
    struct nk_tasker__node *pending;

    pending = nk_tasker__sleep(tasker, node);
    nk_list__add_sorted(&blocker->p__sentinel,
                        &node->p__entry,
                        priority_discriminator);
    return pending;
}

/**
 * \brief       Put the highest priority blocked node into the running state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__unblock_highest(struct nk_tasker *tasker,
                           struct nk_tasker__blocker *blocker)
{
    struct nk_tasker__node *pending;

    pending = NK_COMPILER__CONTAINER_OF(nk_list__next(&blocker->p__sentinel),
                                        struct nk_tasker__node,
                                        p__entry);
    nk_list__remove(&pending->p__entry);

    return nk_tasker__run(tasker, pending);
}

/**
 * \brief       Put all blocked nodes into the running state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__unblock_all(struct nk_tasker *tasker,
                       struct nk_tasker__blocker *blocker)
{
    struct nk_tasker__node *current;

    while (nk_list__is_empty(&blocker->p__sentinel) == false) {
        current = nk_tasker__unblock_highest(tasker, blocker);
    }
    return current;
}
