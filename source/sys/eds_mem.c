/*
 * eds_mem.c
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#include "sys/eds_mem.h"
#include "eds_object.h"
#include "sys/eds_core.h"
#include "eds.h"

static struct eds_object__mem mem__instances_storage[EDS__DEFAULT_MEM_ENTRIES];
struct eds_object__vector mem__instances = EDS_CORE__VECTOR__INITIALIZER(mem__instances_storage)
;

static struct eds_object__mem*
mem__find(size_t size)
{
    for (uint32_t i = 0u; i < eds_core__vector_n_entries(&mem__instances); i++) {
        struct eds_object__mem *mem;

        mem = eds_core__vector_peek(&mem__instances, i);
        if (mem->p__max_size == size) {
            return mem;
        }
    }
    return NULL;
}

void
mem__add(const struct eds_object__mem *mem)
{
    uint32_t index;

    index = 0u;
    for (; index < eds_core__vector_n_entries(&mem__instances); index++) {
        struct eds_object__mem *current;

        current = eds_core__vector_peek(&mem__instances, index);
        if (current->p__max_size > mem->p__max_size) {
            break;
        }
    }
    eds_core__vector_insert(&mem__instances, index, mem);
}

uint32_t
eds_mem__instance_count(void)
{
    return eds_core__vector_n_entries(&mem__instances);
}

struct eds_object__mem*
eds_mem__select(size_t size)
{
    for (uint32_t i = 0u; i < eds_core__vector_n_entries(&mem__instances); i++) {
        struct eds_object__mem *current;

        current = eds_core__vector_peek(&mem__instances, i);
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

eds__error
eds_mem__add_allocator(eds__mem_alloc_fn *alloc,
    eds__mem_dealloc_fn *dealloc,
    void *context,
    size_t max_size)
{
    struct eds_object__mem *mem;
    struct eds_object__mem n_mem;

    mem = mem__find(max_size);
    if (mem != NULL) {
        return EDS__ERROR_ALREADY_EXISTS;
    }
    n_mem.p__alloc = alloc;
    n_mem.p__dealloc = dealloc;
    n_mem.p__context = context;
    n_mem.p__max_size = max_size;
    mem__add(&n_mem);
    return EDS__ERROR_NONE;
}