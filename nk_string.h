/*
 * nk_string.h
 *
 *  Created on: Apr 10, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_NK_STRING_H_
#define NEON_KIT_GENERIC_NK_STRING_H_

#include <stddef.h>
#include <stdbool.h>

#include "nk_array.h"
#include "nk_result.h"

#define NK_ENABLED_STRING

struct nk_string NK_ARRAY__T(char);

#define NK_STRING__INITIALIZER(string_buffer_p)                             \
        NK_ARRAY__INITIALIZER(string_buffer_p)

#define NK_STRING__INITIALIZER_WITH(string_buffer_p, c_string)              \
        NK_ARRAY__INITIALIZER_WITH(string_buffer_p, sizeof(c_string) - 1u)

#define NK_STRING__INITIALIZE(string_p, string_buffer_p)                    \
        nk_string__p__initialize(                                           \
                    (string_p),                                             \
                    (string_buffer_p),                                      \
                    sizeof(string_buffer_p))

#define NK_STRING__BUCKET_T(char_no)                                        \
        {                                                                   \
            struct nk_string string;                                        \
            char string_array[(char_no)];                                   \
        }

#define NK_STRING__BUCKET_INITIALIZER(self)                                 \
        {                                                                   \
            .string = NK_STRING__INITIALIZER((self)->string_array),         \
            .string_array = { 0 }                                           \
        }

#define NK_STRING__BUCKET_INITIALIZER_FROM(self, c_string)                  \
        {                                                                   \
            .string = NK_STRING__INITIALIZER_WITH(                          \
                        (self)->string_array, (c_string)),                  \
            .string_array = c_string                                        \
        }

struct nk_string__find__result
    NK_RESULT__T(size_t);

static inline
void
nk_string__p__initialize(struct nk_string *string, char *string_buffer, size_t string_buffer_size)
{
    NK_ARRAY__INITIALIZE(string, string_buffer_size, string_buffer);
}

static inline size_t
nk_string__length(const struct nk_string *self)
{
    return self->length;
}

static inline size_t
nk_string__size(const struct nk_string *self)
{
    return self->item_no;
}

static inline char
nk_string__char(const struct nk_string *self, size_t index)
{
    return self->items[index];
}

bool
nk_string__is_equal(const struct nk_string *self, const struct nk_string *other);

struct nk_string
nk_string__view(const struct nk_string *self, size_t from, size_t to);

struct nk_string__find__result
nk_string__find(const struct nk_string *self, const struct nk_string *other, size_t from, size_t to);

bool
nk_string__contains(const struct nk_string *self, const struct nk_string *other);

bool
nk_string__endswith(const struct nk_string *self, const struct nk_string *other);

/**
 * \brief       Modifies string \a self and removes trailing characters specified in \a other removed
 */
void
nk_string__rstrip(struct nk_string *self, const struct nk_string *other);

void
nk_string__lower(struct nk_string *self);

void
nk_string__upper(struct nk_string *self);

char nk_char__lower(char character);

char nk_char__upper(char character);

#endif /* NEON_KIT_GENERIC_NK_STRING_H_ */
