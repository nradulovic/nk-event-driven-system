/*
 * eds_mem.c
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#include "eds_mem.h"
#include "eds_object.h"

struct eds_object__mem eds_mem__p__instance[NK_EDS__ALLOCATOR__N_OF_INSTANCES];
uint32_t eds_mem__p__instance_count;

static struct eds_object__mem*
mem__find(size_t size)
{

}

struct eds_object__mem*
eds_mem__select(size_t size)
{
    return NULL;
}

void*
eds_mem__allocate_from(struct eds_object__mem *mem, size_t size)
{
    return NULL;
}

void
eds_mem__deallocate_to(struct eds_object__mem *mem, void *memory)
{

}

nk_eds_error
nk_eds_mem__add_allocator(nk_eds__alloc_fn *alloc,
    nk_eds__dealloc_fn *dealloc,
    void *context,
    size_t max_size)
{
    struct eds_object__mem * mem;

    if ((alloc == NULL) || (dealloc == NULL) || (max_size < NK_EDS__ALLOCATOR__MIN_BYTES)) {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    if (eds_mem__p__instance_count == NK_EDS__ALLOCATOR__N_OF_INSTANCES) {
        return NK_EDS_ERROR__NO_RESOURCE;
    }
    mem = mem__find(max_size);
    if (mem != NULL) {
        return NK_EDS_ERROR__EXISTS;
    }

}
