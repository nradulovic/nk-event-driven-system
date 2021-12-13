/*
 * eds_mem.h
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_EDS_MEM_H_
#define NEON_KIT_GENERIC_SOURCE_EDS_MEM_H_

#include "eds_object.h"
#include "eds.h"

#include <stdint.h>
#include <stddef.h>

extern struct eds_object__vector mem__instances;

uint32_t
eds_mem__instance_count(void);

void
mem__add(const struct eds_object__mem* mem);

struct eds_object__mem*
eds_mem__select(size_t size);

inline void*
eds_mem__allocate_from(struct eds_object__mem *mem, size_t size)
{
    return mem->p__alloc(mem->p__context, size);
}

inline void
eds_mem__deallocate_to(struct eds_object__mem *mem, void *memory)
{
    mem->p__dealloc(mem->p__context, memory);
}

eds__error
eds_mem__add_allocator(eds__mem_alloc_fn *alloc,
    eds__mem_dealloc_fn *dealloc,
    void *context,
    size_t max_size);

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_MEM_H_ */
