/*
 * std_allocator.h
 *
 *  Created on: Feb 2, 2024
 *      Author: nenad
 */

#ifndef STD_ALLOCATOR_H_
#define STD_ALLOCATOR_H_

#include <stddef.h>

#define STD_ALLOCATOR_MAX_SIZE 					SIZE_MAX

void * std_allocator_alloc(void * context, size_t size);

void std_allocator_free(void * context, void * memory);

#endif /* STD_ALLOCATOR_H_ */
