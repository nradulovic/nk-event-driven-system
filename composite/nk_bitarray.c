/*
 * nk_bitarray.c
 *
 *  Created on: May 25, 2021
 *      Author: nenad
 */

#include <string.h>
#include "generic/composite/nk_bitarray.h"
#include "generic/portable/nk_cpu.h"

void nk_bitarray__initialize(struct nk_bitarray * bitarray)
{
    memset(bitarray->field, 0, bitarray->size * 32u);
}

void
nk_bitarray__atomic_set(struct nk_bitarray * bitarray, uint_fast8_t bit_index)
{
    uint_fast8_t mask_i;
    uint_fast8_t field_i;

    mask_i = bit_index >> 5u;
    field_i = bit_index & 0x1fu;
    nk_cpu__bit_set(&bitarray->field[mask_i], field_i);
    nk_cpu__bit_set(&bitarray->mask, mask_i);
}

void
nk_bitarray__atomic_clear(struct nk_bitarray * bitarray, uint_fast8_t bit_index)
{
    uint_fast8_t mask_i;
    uint_fast8_t field_i;
    uint32_t old_field;
    uint32_t new_field;

    mask_i = bit_index >> 5u;
    field_i = bit_index & 0x1fu;
    do {
        nk_cpu__bit_clear(&bitarray->mask, mask_i);
        old_field = bitarray->field[mask_i];
        new_field &= ~(0x1u << field_i);
        if (new_field == 0u) {
            nk_cpu__bit_clear(&bitarray->mask, mask_i);
        }
    } while (old_field != nk_cpu__compare_exchange(&bitarray->field[mask_i], new_field, old_field));
}

void
nk_bitarray__set(struct nk_bitarray * bitarray, uint_fast8_t bit_index)
{
    uint_fast8_t mask_i;
    uint_fast8_t field_i;

    mask_i = bit_index >> 5u;
    field_i = bit_index & 0x1fu;
    bitarray->field[mask_i] |= 0x1u << field_i;
    bitarray->mask |= 0x1u << mask_i;
}

void
nk_bitarray__clear(struct nk_bitarray * bitarray, uint_fast8_t bit_index)
{
    uint_fast8_t mask_i;
    uint_fast8_t field_i;
    uint32_t field;

    mask_i = bit_index >> 5u;
    field_i = bit_index & 0x1fu;
    field = bitarray->field[mask_i];
    field &= ~(0x1u << field_i);
    bitarray->field[mask_i] = field;
    if (field == 0u) {
        bitarray->mask &= ~(0x1u << mask_i);
    }
}

uint_fast32_t
nk_bitarray__ffs(const struct nk_bitarray * bitarray)
{
    uint_fast8_t mask_i;
    uint_fast8_t field_i;

    mask_i = nk_cpu__ffs(bitarray->mask);
    field_i = nk_cpu__ffs(bitarray->field[mask_i]);
    return mask_i * 32u + field_i;
}

uint_fast32_t
nk_bitarray__ffs_clear(struct nk_bitarray * bitarray)
{
    uint_fast8_t mask_i;
    uint_fast8_t field_i;
    uint32_t field;

    mask_i = nk_cpu__ffs(bitarray->mask);
    field_i = nk_cpu__ffs(bitarray->field[mask_i]);
    field = bitarray->field[mask_i];
    field &= ~(0x1u << field_i);
    bitarray->field[mask_i] = field;
    if (field == 0u) {
        bitarray->mask &= ~(0x1u << mask_i);
    }
    return mask_i * 32u + field_i;
}
