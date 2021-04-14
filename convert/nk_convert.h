/*
 * nk_convert.h
 *
 *  Created on: Apr 12, 2021
 *      Author: nenad
 */

#ifndef GENERIC_NK_CONVERT_H_
#define GENERIC_NK_CONVERT_H_

#include <stdint.h>

#include "../nk_result.h"

struct nk_convert__hex_to_bin__result NK_RESULT__T(uint8_t);
struct nk_convert__bin_to_hex__result NK_RESULT__T(char);


struct nk_convert__hex_to_bin__result
nk_convert__hex_to_bin(char character);

struct nk_convert__bin_to_hex__result
nk_convert__bin_to_hex(uint8_t byte);

#endif /* GENERIC_NK_CONVERT_H_ */
