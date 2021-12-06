/*
 * eds_mem.h
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_EDS_MEM_H_
#define NEON_KIT_GENERIC_SOURCE_EDS_MEM_H_

#include "eds_object.h"

#include <stdint.h>
#include <stddef.h>

extern struct eds_object__vector eds_mem__instances;

uint32_t
eds_mem__instance_count(void);

void
eds_mem__add(const struct eds_object__mem* mem);

struct eds_object__mem*
eds_mem__select(size_t size);

inline void*
eds_mem__allocate_from(struct eds_object__mem *mem, size_t size)
{
    return mem->alloc(mem->context, size);
}

inline void
eds_mem__deallocate_to(struct eds_object__mem *mem, void *memory)
{
    mem->dealloc(mem->context, memory);
}

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_MEM_H_ */
