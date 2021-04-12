/*
 * nk_array.h
 *
 *  Created on: Apr 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_NK_ARRAY_H_
#define NEON_KIT_GENERIC_NK_ARRAY_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "nk_bits.h"

#define NK_ENABLED_ARRAY

#define NK_ARRAY__T(item_type)                                              \
        {                                                                   \
            struct nk_array__p__base array_base;                            \
            item_type * items;                                              \
        }

#define NK_ARRAY__INITIALIZER(items_p)                                      \
        {                                                                   \
            .array_base =                                                   \
            {                                                               \
                .item_no = NK_BITS__ARRAY_SIZE(items_p),                    \
            },                                                              \
            .items = items_p                                                \
        }

#define NK_ARRAY__INITIALIZE(array_p, item_no, items_p)                     \
        do {                                                                \
            nk_array__p__initialize(                                        \
                    &(array_p)->array_base,                                 \
                    item_no,                                                \
                    items_p,                                                \
                    nk_array__item_size(array_p) * (item_no));              \
            (array_p)->items = (items_p);                                   \
        } while (0)

#define NK_ARRAY__INITIALIZE_FROM(array_p, item_no, items_p)                \
        do {                                                                \
            nk_array__p__initialize_from(&(array_p)->array_base, item_no);  \
            (array_p)->items = (items_p);                                   \
        } while (0)

#define nk_array__item_size(array_p)                                        \
        sizeof(*(array_p)->items)

#define nk_array__item_no(array_p)                                          \
        nk_array__p__item_no(&(array_p)->array_base)

#define nk_array__ref(array_p, index_s)                                     \
        (&(array_p)->items[(index_s)])

#define NK_ARRAY__BUCKET_T(item_type, item_no)                              \
        {                                                                   \
            struct NK_ARRAY__T(item_type) array;                            \
            item_type buffer[item_no];                                      \
        }

#define NK_ARRAY__BUCKET_NAMED_T(item_type, item_no, type_name)             \
        {                                                                   \
            struct type_name NK_ARRAY__T(item_type) array;                  \
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

#define NK_ARRAY__BUCKET_INITIALIZER_WITH(self, ...)                        \
        {                                                                   \
            .array = NK_ARRAY__INITIALIZER((self)->buffer),                 \
            .buffer =                                                       \
            {                                                               \
                __VA_ARGS__                                                 \
            }                                                               \
        }

#define NK_ARRAY(array_p)                                                   \
        &(array_p)->array

#define NK_ARRAY__BUCKET_INITIALIZE(array_p)                                \
        NK_ARRAY__INITIALIZE(                                               \
                NK_ARRAY(array_p),                                          \
                NK_BITS__ARRAY_SIZE((array_p)->buffer),                     \
                (array_p)->buffer)

/** @brief      Array object structure.
 */
struct nk_array__p__base
{
    size_t item_no; /* Number of items in buffer */
};

static inline void
nk_array__p__initialize(
        struct nk_array__p__base * array_base,
        size_t item_no,
        void * items_buffer,
        size_t items_buffer_size)
{
    array_base->item_no = item_no;
    memset(items_buffer, 0, items_buffer_size);
}

static inline void
nk_array__p__initialize_from(
        struct nk_array__p__base * array_base,
        size_t item_no)
{
    array_base->item_no = item_no;
}

static inline size_t
nk_array__p__item_no(const struct nk_array__p__base * array_base)
{
    return array_base->item_no;
}

#endif /* NEON_KIT_GENERIC_NK_ARRAY_H_ */
