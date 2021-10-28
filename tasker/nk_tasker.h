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

struct nk_tasker
{
    uint32_t p__bitmap;
    struct nk_tasker__p__sentinels
        NK_FARRAY__T(struct nk_list*, NK_TASKER__MAX_PRIO + 1u)
    p__sentinels;
};

struct nk_tasker__blocker
{
    struct nk_list p__sentinel;
};

struct nk_tasker__node
{
    struct nk_list p__entry;
    uint_fast8_t p__priority;
};

/**
 * A node can have the following states
 * - SLEEPING
 * - RUNNING
 * - BLOCKING
 */

void
nk_tasker__init(struct nk_tasker * tasker);

nk_tasker__blocker__init(struct nk_tasker__blocker * blocker);

void
nk_tasker__node__init(struct nk_tasker__node * node, uint_fast8_t priority);

static inline uint_fast8_t
nk_tasker__node__priority(const struct nk_tasker__node * node)
{
    return node->p__priority;
}

/**
 * \brief       Put a sleeping node into the running state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__run(struct nk_tasker *tasker, struct nk_tasker__node *node);

/**
 * \brief       Put a running node into the sleeping state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__sleep(struct nk_tasker *tasker, struct nk_tasker__node *node);

/**
 * \brief       Put the highest priority running node into the blocked state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__block(struct nk_tasker *tasker, struct nk_tasker__blocker *blocker);

/**
 * \brief       Put the highest priority blocked node into the running state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__unblock_highest(struct nk_tasker *tasker, struct nk_tasker__blocker *blocker);

/**
 * \brief       Put all blocked nodes into the running state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__unblock_all(struct nk_tasker *tasker, struct nk_tasker__blocker *blocker);

#if defined(__cplusplus)
}
#endif

#endif /* NEON_KIT_GENERIC_NK_TASKER_H_ */
