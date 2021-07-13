/*
 * nk_bitarray.h
 *
 *  Created on: May 25, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_COMPOSITE_NK_BITARRAY_H_
#define NEON_KIT_GENERIC_COMPOSITE_NK_BITARRAY_H_

#include <stddef.h>
#include <stdint.h>
#include "generic/common/nk_bits.h"

#define NK_BITARRAY__T(bitsize) \
        {\
            uint32_t fields[1u + NK_BITS__DIVIDE_ROUNDUP((bitsize), 32u)]; \
        }

#define NK_BITARRAY__INITIALIZER(self) \
        { \
            .fields = { 0 } \
        }

#define NK_BITARRAY__INITIALIZE(self) \
            memset(&(self)->fields[0], 0, sizeof(&(self)->fields))

#define NK_BITARRAY__SET(self, a_bit) \
        nk_bitarray__p__set(&(self)->fields, (a_bit))


struct nk_bitarray
{
    uint32_t mask;
    uint32_t * field;
    size_t size;
};

#define NK_BITARRAY__BUCKET_T(bitsize)                                      \
        {                                                                   \
            struct nk_bitarray bitarray;                                    \
            uint_fast32_t fields[NK_BITS__DIVIDE_ROUNDUP((bitsize), 32u)];  \
        }

#define NK_BITARRAY__BUCKET_INITIALIZER(self)                               \
        {                                                                   \
            .bitarray =                                                     \
            {                                                               \
                .mask = 0u,                                                 \
                .field = &(self)->fields[0],                                \
                .size = NK_BITS__ARRAY_SIZE((self)->fields) / 32u           \
            },                                                              \
            .fields = { 0 }                                                 \
        }

#define NK_BITARRAY__BUCKET_INITIALIZE(self)                                \
        do {                                                                \
            (self)->bitarray.mask = 0u;                                     \
            (self)->bitarray.field = &(self)->fields[0];                    \
            (self)->bitarray.size =                                         \
                    NK_BITS__ARRAY_SIZE((self)->fields) / 32u;              \
            nk_bitarray__initialize(self);                                  \
        } while (0)


void
nk_bitarray__initialize(struct nk_bitarray * bitarray);

void
nk_bitarray__atomic_set(struct nk_bitarray * bitarray, uint_fast8_t bit_index);

void
nk_bitarray__atomic_clear(struct nk_bitarray * bitarray, uint_fast8_t bit_index);

void
nk_bitarray__set(struct nk_bitarray * bitarray, uint_fast8_t bit_index);

void
nk_bitarray__clear(struct nk_bitarray * bitarray, uint_fast8_t bit_index);

uint_fast32_t
nk_bitarray__ffs(const struct nk_bitarray * bitarray);

uint_fast32_t
nk_bitarray__ffs_clear(struct nk_bitarray * bitarray);

#endif /* NEON_KIT_GENERIC_COMPOSITE_NK_BITARRAY_H_ */
