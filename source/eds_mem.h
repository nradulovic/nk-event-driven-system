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

extern struct eds_object__mem eds_mem__p__instance[];
extern uint32_t eds_mem__p__instance_count;

inline uint32_t
eds_mem__instance_count(void)
{
    return eds_mem__p__instance_count;
}

void
eds_mem__add(const struct eds_object__mem* mem);

struct eds_object__mem*
eds_mem__select(size_t size);

void*
eds_mem__allocate_from(struct eds_object__mem *mem, size_t size);

void
eds_mem__deallocate_to(struct eds_object__mem *mem, void *memory);

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_MEM_H_ */
