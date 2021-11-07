/*
 * nk_bit_array.h
 *
 *  Created on: Oct 23, 2021
 *      Author: Nenad.Radulovic
 */

#ifndef GENERIC_COMPOSITE_NK_SBITARRAY_H_
#define GENERIC_COMPOSITE_NK_SBITARRAY_H_

#include <stdint.h>

#include "generic/portable/nk_cpu.h"

struct nk_sbitarray
{
    uint32_t bits;
};

static inline
void nk_sbitarray__init(struct nk_sbitarray * ba)
{
    ba->bits = 0u;
}

static inline
void nk_sbitarray__set(struct nk_sbitarray * ba, uint_fast8_t bit_index)
{
    ba->bits |= 0x1u << bit_index;
}

static inline
void nk_sbitarray__clear(struct nk_sbitarray * ba, uint_fast8_t bit_index)
{
    ba->bits &= ~(0x1u << bit_index);
}

static inline
uint_fast8_t nk_sbitarray__msbs(struct nk_sbitarray * ba)
{
    return nk_cpu__ffs(ba->bits);
}

static inline
uint_fast8_t nk_sbitarray__msbs_clear(struct nk_sbitarray * ba)
{
    uint_fast8_t ret;

    ret = nk_sbitarray__msbs(ba);
    nk_sbitarray__clear(ba, ret);
    return ret;
}

#endif /* GENERIC_COMPOSITE_NK_SBITARRAY_H_ */
