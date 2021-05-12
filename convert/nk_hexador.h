/*
 * hexador.h
 *
 *  Created on: Apr 12, 2021
 *      Author: nenad
 */

#ifndef GENERIC_CONVERT_NK_HEXADOR_H_
#define GENERIC_CONVERT_NK_HEXADOR_H_

#include "generic/nk_result.h"

struct nk_string;
struct nk_types__array__u8;


struct nk_result__size
nk_hexador__to_bin(const struct nk_string *string,
                   struct nk_types__array__u8 *buffer);

struct nk_result__size
nk_hexador__to_hex(const struct nk_types__array__u8 *buffer,
                   struct nk_string *string);

#endif /* GENERIC_CONVERT_NK_HEXADOR_H_ */
