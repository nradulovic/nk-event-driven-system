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

#include "nk_eds.h"
#include "eds_port.h"

struct eds_object__list
{
    struct eds_object__list * p__next;
    struct eds_object__list * p__prev;
};

struct eds_object__mem
{
    void * (*alloc)(size_t);
    void (* dealloc)(void *);
    void * context;
    size_t max_size;
};

struct eds_object__equeue
{
    uint32_t head;
    uint32_t tail;
    uint32_t size;
    struct eds_object__event * storage;
};

struct eds_object__sm_executor
{
    eds_sm__state_fn * p__current;
    void * p__workspace;
};

struct eds_object__escheduler
{
    uint32_t p__prio_group;
    struct eds_object__list p__prio_groups[32];
};

struct eds_object__escheduler_node
{
    struct eds_object__list p__entry;
    eds_sm__prio p__prio;
};

struct eds_object__event
{
    uint32_t p__id;
    size_t p__size;
    struct eds_object__mem * p__mem;
    uint32_t p__ref_count;
};

struct eds_object__sm
{
    struct eds_object__sm_executor p__sm_executor;
    struct eds_object__equeue p__equeue;
    struct eds_object__escheduler_node p__node;
    struct eds_object__epa * p__epa;
    struct eds_object__mem * p__mem;
    const char * p__name;
};

struct eds_object__epa
{
    struct eds_object__escheduler scheduler;
    struct eds_port__sleep sleep;
};

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_OBJECT_H_ */
