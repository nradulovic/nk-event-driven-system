/*
 * nk_tasker.c
 *
 *  Created on: May 8, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 */

#include "generic/tasker/nk_tasker.h"

#include <stddef.h>

void
nk_tasker__init(struct nk_tasker * tasker)
{
    tasker->p__bitmap = 0u;
    for (size_t i = 0u; i < NK_FARRAY__SIZE(tasker->p__sentinels); i++) {
        tasker->p__sentinels.items[i] = NULL;
    }
}

nk_tasker__blocker__init(struct nk_tasker__blocker * blocker)
{
    nk_list__init(&blocker->p__sentinel);
}

void
nk_tasker__node__init(struct nk_tasker__node * node, uint_fast8_t priority)
{
    nk_list__init(&node->p__entry);
    node->p__priority = priority;
}

/**
 * \brief       Put a sleeping node into the running state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__run(struct nk_tasker *tasker, struct nk_tasker__node *node)
{

}

/**
 * \brief       Put a running node into the sleeping state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__sleep(struct nk_tasker *tasker, struct nk_tasker__node *node)
{

}

/**
 * \brief       Put the highest priority running node into the blocked state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__block(struct nk_tasker *tasker, struct nk_tasker__blocker *blocker)
{

}

/**
 * \brief       Put the highest priority blocked node into the running state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__unblock_highest(struct nk_tasker *tasker, struct nk_tasker__blocker *blocker)
{

}

/**
 * \brief       Put all blocked nodes into the running state
 * \return      A new node with highest priority which is in running state.
 */
struct nk_tasker__node*
nk_tasker__unblock_all(struct nk_tasker *tasker, struct nk_tasker__blocker *blocker)
{

}

