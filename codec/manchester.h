/*
 * MANCHESTER.h
 *
 *  Created on: Apr 13, 2021
 *      Author: nenad
 */

#ifndef GENERIC_CODEC_MANCHESTER_H_
#define GENERIC_CODEC_MANCHESTER_H_

#include "nk_error.h"
#include "nk_varray.h"

enum nk_error manchester__encode__biphasel(const struct nk_varray__u8 *source,
                                           struct nk_varray__bool *encoded);

#endif /* GENERIC_CODEC_MANCHESTER_H_ */
