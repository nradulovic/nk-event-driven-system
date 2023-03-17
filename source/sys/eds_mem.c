/*
 * eds_mem.c
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#include "sys/eds_mem.h"
#include "sys/eds_core.h"
#include "sys/eds_state.h"
#include "eds.h"

extern inline void*
eds_mem__allocate_from(struct eds_object__mem * mem, size_t size);

extern inline void
eds_mem__deallocate_to(struct eds_object__mem * mem, void * memory);

struct eds_object__mem*
eds_mem__find(const struct eds_object__vector * vector, size_t size)
{
    uint32_t entries = eds_core__vector_n_entries(vector);
    for (uint32_t i = 0u; i < entries; i++) {
        struct eds_object__mem * mem;

        mem = eds_core__vector_peek(vector, i);
        if (mem->p__max_size >= size) {
            return mem;
        }
    }
    return NULL;
}

eds__error
eds__mem_add_allocator(eds__mem_alloc_fn * alloc,
    eds__mem_dealloc_fn * dealloc,
    void * context,
    size_t max_size)
{
    struct eds_object__mem * existing_mem;
    struct eds_object__mem new_entry;

    if ((alloc == NULL) || (dealloc == NULL) || (max_size < EDS__DEFAULT_MEM_MIN_BYTES)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_state__has_started) {
        return EDS__ERROR_NO_PERMISSION;
    }
    if (eds_core__vector_is_full(&eds_state__mem_instances) == true) {
        return EDS__ERROR_NO_RESOURCE;
    }
    existing_mem = eds_mem__find(&eds_state__mem_instances, max_size);
    if (existing_mem != NULL) {
        return EDS__ERROR_ALREADY_EXISTS;
    }
    /* entry init */
    new_entry.p__alloc = alloc;
    new_entry.p__dealloc = dealloc;
    new_entry.p__context = context;
    new_entry.p__max_size = max_size;
    /* entry add to vector */
    {
        uint32_t index;

        index = 0u;
        for (; index < eds_core__vector_n_entries(&eds_state__mem_instances); index++) {
            struct eds_object__mem * current;

            current = eds_core__vector_peek(&eds_state__mem_instances, index);
            if (current->p__max_size > max_size) {
                break;
            }
        }
        eds_core__vector_insert(&eds_state__mem_instances, index, &new_entry);
    }
    return EDS__ERROR_NONE;
}
