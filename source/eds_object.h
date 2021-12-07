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
    (*alloc)(void *, size_t);
    void
    (*dealloc)(void*, void*);
    void *context;
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

/**
 * @brief       Virtual timer structure
 *
 * @note        All elements of this structure are private members. This implementation detail is
 *              only exposed so the structure can be allocated.
 */
struct eds_object__tmr
{
    struct eds_object__list p__list;                        ///< Linked list entry.
    uint32_t p__n_rtick;                                    ///< Relative ticks.
    uint32_t p__n_itick;                                    ///< Initial ticks.
    void (*p__fn)(struct eds_object__tmr *);                ///< Callback function.
};

struct eds_object__escheduler
{
    uint32_t p__prio_group;
    struct eds_object__list p__prio_groups[32];
};

struct eds_object__etask
{
    struct eds_object__list p__entry;
    uint_fast8_t p__prio;
};

struct eds_object__evt
{
    uint32_t p__id;
    size_t p__size;
    struct eds_object__mem *p__mem;
    uint32_t p__ref_count;
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
    struct eds_object__smp p__sm;
    struct eds_object__equeue p__equeue;
    struct eds_object__etask p__etask;
    struct eds_object__mem *p__mem;
    struct eds_object__epn *p__epn;
    const char *p__name;
};

struct eds_object__epn
{
    struct eds_object__escheduler p__scheduler;
    struct eds_port__sleep p__sleep;
};

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_OBJECT_H_ */
