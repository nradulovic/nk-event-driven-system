/*
 * nk_kernel.h
 *
 *  Created on: May 8, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 *  08/05/2021: (nbr) Initial CPU interface definition
 */
/**
 * @file
 * @brief   Neon Kit CPU (KERNEL)
 * @{
 */

#ifndef NEON_KIT_GENERIC_NK_CPU_H_
#define NEON_KIT_GENERIC_NK_CPU_H_

#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @brief   Forward declaration of task instance
 */
struct nk_task;

/**
 * @brief   Forward declaration of CPU context structure
 */
struct nk_cpu__context;


struct nk_cpu__isr
{
    union nk_cpu__isr__variants
    {
        uint32_t u32;
        void * vp;
    } variant;
};

uint32_t
nk_cpu__instruction_count(void);

void
nk_cpu__create_context(struct nk_task * task);

void
nk_cpu__isr__disable(struct nk_cpu__isr * isr);

void
nk_cpu__isr__enable(struct nk_cpu__isr * isr);

bool
nk_cpu__isr__is_enabled(struct nk_cpu__isr * isr);

uint_fast8_t
nk_cpu__ffs(uint32_t);

void
nk_cpu__bit_set(uint32_t * address, uint_fast8_t bit_index);

void
nk_cpu__bit_clear(uint32_t * address, uint_fast8_t bit_index);

uint32_t
nk_cpu__compare_exchange(uint32_t * address, uint32_t new_value, uint32_t old_value);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
#endif /* NEON_KIT_GENERIC_NK_CPU_H_ */
