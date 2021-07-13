/*
 * nk_convert.h
 *
 *  Created on: Apr 12, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 *
 *  08/05/2021: (nbr) Hex functions will convert whole binary byte.
 *  25/05/2021: (nbr) Added float to uint32 converter functions.
 */

#ifndef GENERIC_CONVERT_NK_CONVERT_H_
#define GENERIC_CONVERT_NK_CONVERT_H_

#include <string.h>
#include <stdint.h>

#include "generic/composite/nk_result.h"

#define NK_ENABLED_CONVERT

struct nk_convert__hex
{
    char msb;
    char lsb;
};

struct nk_result__u8
nk_convert__hex_to_bin(struct nk_convert__hex hex);

struct nk_convert__hex
nk_convert__bin_to_hex(uint8_t byte);

struct nk_result__u32
nk_convert__str_to_u32(const struct nk_string * string);

static inline uint32_t
nk_convert__f_to_u32(float value)
{
    uint32_t retval;

    memcpy(&retval, &value, sizeof(retval));

    return retval;
}

static inline float
nk_convert__u32_to_f(uint32_t value)
{
    float retval;

    memcpy(&retval, &value, sizeof(retval));

    return retval;
}

#endif /* GENERIC_CONVERT_NK_CONVERT_H_ */
