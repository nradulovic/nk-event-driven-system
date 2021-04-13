/*
 * nk_varray.h
 *
 *  Created on: Apr 13, 2021
 *      Author: nenad
 */

#ifndef GENERIC_NK_VARRAY_H_
#define GENERIC_NK_VARRAY_H_

#include <stddef.h>

#include "nk_bits.h"
#include "nk_array.h"

#define NK_VARRAY__T(item_type)                                             \
		{                                                                   \
            struct NK_ARRAY__T(item_type) array;                            \
            size_t length;                                                  \
		}

#define NK_VARRAY__INITIALIZE(array_p, a_item_no, items_p)                  \
        do {                                                                \
            NK_ARRAY__INITIALIZE_FROM(&(array_p)->array,                    \
                      (a_item_no),                                          \
                      (items_p));                                           \
            (array_p)->length = (a_item_no);                                \
        } while (0)

#define NK_VARRAY__INITIALIZE_WINDOW(view_p, array_p, from, to)               \
        do {                                                                \
            size_t view_from = MIN((from), (array_p)->length);              \
            size_t view_to = MIN((to), (array_p)->length);                  \
            size_t length = view_to - view_from;                            \
            NK_VARRAY__INITIALIZE((view_p),                                 \
                      length,                                               \
                      &(array_p)->array.items[view_from]);                  \
        } while (0)

struct nk_varray__u8 NK_VARRAY__T(uint8_t);
struct nk_varray__u16 NK_VARRAY__T(uint16_t);
struct nk_varray__u32 NK_VARRAY__T(uint32_t);

struct nk_varray__i8 NK_VARRAY__T(int8_t);
struct nk_varray__i16 NK_VARRAY__T(int16_t);
struct nk_varray__i32 NK_VARRAY__T(int32_t);

struct nk_varray__bool NK_VARRAY__T(bool);
struct nk_varray__size NK_VARRAY__T(size_t);
struct nk_varray__voidp NK_VARRAY__T(void *);

#endif /* GENERIC_NK_VARRAY_H_ */
