/*
 * MANCHESTER.h
 *
 *  Created on: Apr 13, 2021
 *      Author: nenad
 */

#ifndef GENERIC_CODEC_NK_MANCHESTER_H_
#define GENERIC_CODEC_NK_MANCHESTER_H_

#include "generic/nk_error.h"

struct nk_types__array__u8;
struct nk_types__array__bool;

#define NK_ENABLED_MANCHESTER

enum nk_error
nk_manchester__encode__biphasel(const struct nk_types__array__u8 *source, struct nk_types__array__bool *encoded);

enum nk_error
nk_manchester__decode__biphasel(const struct nk_types__array__bool *source, struct nk_types__array__u8 *decoded);

#endif /* GENERIC_CODEC_NK_MANCHESTER_H_ */
