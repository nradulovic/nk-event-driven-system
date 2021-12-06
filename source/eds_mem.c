/*
 * eds_mem.c
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#include "eds_mem.h"
#include "eds_object.h"
#include "eds_core.h"

static struct eds_object__mem eds_mem__p__instance[NK_EDS__ALLOCATOR__N_OF_INSTANCES];
struct eds_object__vector eds_mem__instances = EDS_CORE__VECTOR__INITIALIZER(eds_mem__p__instance);

static struct eds_object__mem*
mem__find(size_t size)
{
    for (uint32_t i = 0u; i < eds_core__vector__length(&eds_mem__instances); i++) {
        struct eds_object__mem * mem;

        mem = eds_core__vector__peek(&eds_mem__instances, i);
        if (mem->p__max_size == size) {
            return mem;
        }
    }
    return NULL;
}

uint32_t
eds_mem__instance_count(void)
{
    return eds_core__vector__length(&eds_mem__instances);
}

void
eds_mem__add(const struct eds_object__mem* mem)
{
    uint32_t index;

    index = 0u;
    for (; index < eds_core__vector__length(&eds_mem__instances); index++) {
        struct eds_object__mem * current;

        current = eds_core__vector__peek(&eds_mem__instances, index);
        if (current->p__max_size > mem->p__max_size) {
            break;
        }
    }
    eds_core__vector__insert(&eds_mem__instances, index, mem);
}

struct eds_object__mem*
eds_mem__select(size_t size)
{
    for (uint32_t i = 0u; i < eds_core__vector__length(&eds_mem__instances); i++) {
        struct eds_object__mem * current;

        current = eds_core__vector__peek(&eds_mem__instances, i);
        if (current->p__max_size >= size) {
            return current;
        }
    }
    return NULL;
}

extern inline void*
eds_mem__allocate_from(struct eds_object__mem *mem, size_t size);

extern inline void
eds_mem__deallocate_to(struct eds_object__mem *mem, void *memory);

nk_eds_error
nk_eds_mem__add_allocator(nk_eds__alloc_fn *alloc,
    nk_eds__dealloc_fn *dealloc,
    void *context,
    size_t max_size)
{
    struct eds_object__mem * mem;
    struct eds_object__mem n_mem;

    if ((alloc == NULL) || (dealloc == NULL) || (max_size < NK_EDS__ALLOCATOR__MIN_BYTES)) {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    if (eds_core__vector__is_full(&eds_mem__instances) == true) {
        return NK_EDS_ERROR__NO_RESOURCE;
    }
    mem = mem__find(max_size);
    if (mem != NULL) {
        return NK_EDS_ERROR__EXISTS;
    }
    n_mem.alloc = alloc;
    n_mem.dealloc = dealloc;
    n_mem.context = context;
    n_mem.p__max_size = max_size;
    eds_mem__add(&n_mem);
    return NK_EDS_ERROR__NONE;
}
