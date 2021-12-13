/*
 * nk_eds_object.h
 *
 *  Created on: Dec 2, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_NK_EDS_OBJECT_H_
#define NEON_KIT_GENERIC_SOURCE_NK_EDS_OBJECT_H_

#include <stdint.h>
#include <stddef.h>

/* Include port specific type definitions */
#include "eds_port/eds_port_definition.h"

/* Include NK EDS API integral types */
#include "eds.h"

struct eds_object__vector
{
    uint32_t p__n_entries;
    uint32_t p__n_size;
    size_t p__item_size;
    void *p__entries;
};

struct eds_object__list
{
    struct eds_object__list *p__next;
    struct eds_object__list *p__prev;
};

struct eds_object__mem
{
    void*
    (*p__alloc)(void*, size_t);
    void
    (*p__dealloc)(void*, void*);
    void *p__context;
    size_t p__max_size;
};

struct eds_object__queue
{
    uint32_t p__head;
    uint32_t p__tail;
    uint32_t p__n_entries;
    uint32_t p__n_free;
    void **p__storage;
};

struct eds_object__equeue
{
    struct eds_object__queue queue;
};

enum eds_object__tmr_state
{
    EDS_OBJECT__TMR_STATE__DORMENT,
    EDS_OBJECT__TMR_STATE__PENDING,
    EDS_OBJECT__TMR_STATE__ACTIVE
};

struct eds_object__tmr
{
    struct eds_object__list p__active;
    struct eds_object__list p__pending;
};

/**
 * @brief       Virtual timer structure
 *
 * @note        All elements of this structure are private members.
 */
struct eds_object__tmr_node
{
    struct eds_object__list p__list;                        ///< Linked list entry.
    uint32_t p__n_rtick;                                    ///< Relative ticks.
    uint32_t p__n_itick;                                    ///< Initial ticks.
    void
    (*p__fn)(struct eds_object__tmr_node*);                      ///< Callback function.
    enum eds_object__tmr_state p__state;
};


struct eds_object__etm
{
    struct eds_object__tmr p__tmr;
};

struct eds_object__etm_node
{
    struct eds_object__tmr_node p__node;
    const struct eds_object__evt *p__evt;
    struct eds_object__epa *p__epa;
    struct eds_object__mem *p__mem;
};

struct eds_object__tasker
{
    uint32_t p__pending_group;
    struct eds_object__list p__pending_groups[32];
    struct eds_object__tasker_node *current;
};

struct eds_object__tasker_node
{
    struct eds_object__list p__list;
    uint_fast8_t p__prio;
};

struct eds_object__evt
{
    uint32_t p__id;
    uint32_t p__ref_count;
    size_t p__size;
    struct eds_object__mem *p__mem;
};

typedef eds__sm_action eds_object__smp_action;

/**
 * @brief       SMP state function type
 */
typedef eds__sm_state eds_object__smp_state;

struct eds_object__smp
{
    eds_object__smp_state *p__state;
    void *p__workspace;
};

struct eds_object__epa
{
    struct eds_object__tasker_node p__task;
    struct eds_object__smp p__smp;
    struct eds_object__equeue p__equeue;
    struct eds_object__epn *p__epn;
    struct eds_object__mem *p__mem;
    const char *p__name;
};

struct eds_object__epn
{
    struct eds_object__list p__list;
    struct eds_object__tasker p__tasker;
    struct eds_port__sleep p__sleep;
    bool p__should_run;
    struct eds_object__etm p__etm;
    struct eds_object__mem *p__mem;
    const char *p__name;
};

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_OBJECT_H_ */
