/*
 * nk_array.h
 *
 *  Created on: Apr 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_NK_ARRAY_H_
#define NEON_KIT_GENERIC_NK_ARRAY_H_

#include <stddef.h>
#include "nk_bits.h"

#define NK_ENABLED_ARRAY

#define NK_ARRAY__T(item_type)                                              \
        {                                                                   \
            size_t item_no; /* Number of items in buffer */                 \
            size_t length;                                                  \
            item_type * items;                                              \
        }

#define NK_ARRAY__INITIALIZER(items_p)                                      \
        {                                                                   \
            .item_no = NK_BITS__ARRAY_SIZE(items_p),                        \
            .length = 0u,                                                   \
            .items = items_p                                                \
        }

#define NK_ARRAY__INITIALIZER_WITH(items_p, a_length)                       \
        {                                                                   \
            .item_no = NK_BITS__ARRAY_SIZE(items_p),                        \
            .length = (a_length),                                           \
            .items = items_p                                                \
        }

#define NK_ARRAY__INITIALIZE(array_p, a_item_no, items_p)                   \
        do {                                                                \
            (array_p)->item_no = (a_item_no);                               \
            (array_p)->items = (items_p);                                   \
            (array_p)->length = 0u;                                         \
        } while (0)

#define NK_ARRAY__INITIALIZE_WINDOW(window_p, array_p, from, to)            \
        do {                                                                \
            size_t view_from = MIN((from), (array_p)->length);              \
            size_t view_to = MIN((to), (array_p)->length);                  \
            size_t length = view_to - view_from;                            \
            (window_p)->item_no = length;                                   \
            (window_p)->length = length;                                    \
            (window_p)->items = &(array_p)->items[view_from];               \
        } while (0)

#define nk_array__item_size(array_p)                                        \
        sizeof(*(array_p)->items)

#define NK_ARRAY__BUCKET_T(item_type, item_no)                              \
        {                                                                   \
            struct NK_ARRAY__T(item_type) array;                            \
            item_type buffer[item_no];                                      \
        }

#define NK_ARRAY__BUCKET_TYPED_T(item_type, item_no, array_type)            \
        {                                                                   \
            array_type array;                                               \
            item_type buffer[item_no];                                      \
        }

#define NK_ARRAY__BUCKET_INITIALIZER(self)                                  \
        {                                                                   \
            .array = NK_ARRAY__INITIALIZER((self)->buffer),                 \
            .buffer =                                                       \
            {                                                               \
                0                                                           \
            }                                                               \
        }

#define NK_ARRAY__BUCKET_INITIALIZER_WITH(self, a_length, ...)              \
        {                                                                   \
            .array = NK_ARRAY__INITIALIZER_WITH(                            \
                    (self)->buffer, (a_length)),                            \
            .buffer = __VA_ARGS__                                           \
        }

#define NK_ARRAY__BUCKET_INITIALIZE(array_p)                                \
        NK_ARRAY__INITIALIZE(                                               \
                &((array_p)->array),                                        \
                NK_BITS__ARRAY_SIZE((array_p)->buffer),                     \
                (array_p)->buffer)

#define NK_ARRAY__FREE(array_p)                                             \
        ((array_p)->item_no - (array_p)->length)

#endif /* NEON_KIT_GENERIC_NK_ARRAY_H_ */
