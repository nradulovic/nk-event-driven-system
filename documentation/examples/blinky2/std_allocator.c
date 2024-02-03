/*
 * std_allocator.c
 *
 *  Created on: Feb 2, 2024
 *      Author: nenad
 */

/* Implements */
#include "std_allocator.h"

/* Depends */
#include <stdlib.h>

void * std_allocator_alloc(void * context, size_t size)
{
    (void)context;

    return malloc(size);
}

void std_allocator_free(void * context, void * memory)
{
    (void)context;

    free(memory);
}
