/*
 * nk_types.h
 *
 *  Created on: Apr 12, 2021
 *      Author: nenad
 */

#ifndef GENERIC_NK_TYPES_H_
#define GENERIC_NK_TYPES_H_

#include <stdint.h>

#include "nk_array.h"
#include "nk_varray.h"

struct nk_types__array__u8 NK_ARRAY__T(uint8_t);
struct nk_types__array__u16 NK_ARRAY__T(uint16_t);
struct nk_types__array__u32 NK_ARRAY__T(uint32_t);

struct nk_types__array__i8 NK_ARRAY__T(int8_t);
struct nk_types__array__i16 NK_ARRAY__T(int16_t);
struct nk_types__array__i32 NK_ARRAY__T(int32_t);

struct nk_types__array__size NK_ARRAY__T(size_t);
struct nk_types__array__voidp NK_ARRAY__T(void *);

struct nk_types__buffer_array NK_ARRAY__T(uint8_t);

struct nk_types__varray__u8 NK_VARRAY__T(uint8_t);
struct nk_types__varray__u16 NK_VARRAY__T(uint16_t);
struct nk_types__varray__u32 NK_VARRAY__T(uint32_t);

struct nk_types__varray__i8 NK_VARRAY__T(int8_t);
struct nk_types__varray__i16 NK_VARRAY__T(int16_t);
struct nk_types__varray__i32 NK_VARRAY__T(int32_t);

struct nk_types__varray__size NK_VARRAY__T(size_t);
struct nk_types__varray__voidp NK_VARRAY__T(void *);

#endif /* GENERIC_NK_TYPES_H_ */
