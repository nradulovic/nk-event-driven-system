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

inline void*
eds_mem__context(const struct eds_object__mem * mem)
{
    return mem->p__context;
}

inline size_t
eds_mem__max_size(const struct eds_object__mem * mem)
{
    return mem->p__max_size;
}

inline eds__mem_alloc_fn*
eds_mem__alloc_fn(const struct eds_object__mem * mem)
{
    return mem->p__alloc;
}

inline eds__mem_dealloc_fn*
eds_mem__dealloc_fn(const struct eds_object__mem * mem)
{
    return mem->p__dealloc;
}

inline void*
eds_mem__allocate_from(struct eds_object__mem * mem, size_t size)
{
    return mem->p__alloc(mem->p__context, size);
}

inline void
eds_mem__deallocate_to(struct eds_object__mem * mem, void * memory)
{
    mem->p__dealloc(mem->p__context, memory);
}

struct eds_object__mem*
eds_mem__find(const struct eds_object__vector * vector, size_t size);

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_MEM_H_ */
