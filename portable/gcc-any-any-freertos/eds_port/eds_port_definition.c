/*
 * eds_port_definition.c
 *
 *  Created on: Dec 14, 2021
 *      Author: nenad
 */

#include "eds_port_definition.h"

#include <stdint.h>
#include <stddef.h>

#define ALIGN_UP(num, align)                                       \
        (((num) + (align) - 1u) & ~((align) - 1u))

void
eds_port__critical_local_lock(struct eds_port__critical_local * critical)
{
}

void
eds_port__critical_local_unlock(struct eds_port__critical_local * critical)
{
}


void
eds_port__sleep_local_init(struct eds_port__sleep_local * sleep)
{
}

void
eds_port__sleep_local_wait(struct eds_port__sleep_local * sleep)
{
}

void
eds_port__sleep_local_signal(struct eds_port__sleep_local * sleep)
{
}

uint_fast8_t
eds_port__ffs(uint32_t value)
{
    return (uint_fast8_t)(31u - (unsigned)__builtin_clz(value));
}

size_t
eds_port__align_up(size_t non_aligned_value)
{
    return ALIGN_UP(non_aligned_value, sizeof(void*));
}

uint32_t
eds_port__tick_duration_ms(void)
{
    return 1u;
}

uint32_t
eds_port__tick_from_ms(uint32_t ms)
{
    return ms;
}

void
eds_port__timer_init(struct eds_port__timer * timer)
{

}

void
eds_port__timer_start(struct eds_port__timer * timer)
{

}

void
eds_port__timer_stop(struct eds_port__timer * timer)
{
    
}

void
eds_port__init(void)
{
}
