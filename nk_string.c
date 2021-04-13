/*
 * nk_string.c
 *
 *  Created on: Apr 11, 2021
 *      Author: nenad
 */
#include <ctype.h>

#include "nk_bits.h"
#include "nk_string.h"

bool
nk_string__is_equal(const struct nk_string *self, const struct nk_string *other)
{
    if (self->length != other->length) {
        return false;
    }
    for (size_t i = 0u; i < self->length; i++) {
        if (self->array.items[i] != other->array.items[i]) {
            return false;
        }
    }
    return true;
}

struct nk_string
nk_string__view(const struct nk_string *self, size_t from, size_t to)
{
    size_t view_from = MIN(from, self->length);
    size_t view_to = MIN(to, self->length);
    struct nk_string view;

    NK_ARRAY__INITIALIZE_FROM(&view.array, view_to - view_from, &self->array.items[view_from]);
    view.length = view_to - view_from;
    return view;
}

struct nk_string__find__result
nk_string__find(const struct nk_string *self, const struct nk_string *other, size_t from, size_t to)
{
    struct nk_string__find__result result = { .error = NK_ERROR__NOT_FOUND, .value = 0u };

    const struct nk_string source = nk_string__view(self, from, to);
    if (other->length < source.length) {
        for (size_t i = 0u; i < source.length - other->length; i++) {
            const struct nk_string view = nk_string__view(&source, i, i + other->length);

            if (nk_string__is_equal(&view, other)) {
                result = (struct nk_string__find__result ) { .error = NK_ERROR__OK, .value = i };
                break;
            }
        }
    }
    return result;
}

bool
nk_string__contains(const struct nk_string *self, const struct nk_string *other)
{
    struct nk_string__find__result find = nk_string__find(self, other, 0u, SIZE_MAX);

    if (find.error == NK_ERROR__OK) {
        return true;
    } else {
        return false;
    }
}

bool
nk_string__endswith(const struct nk_string *self, const struct nk_string *other)
{
    if (other->length > self->length) {
        return false;
    }
    const struct nk_string view = nk_string__view(self, self->length - other->length, self->length);
    if (nk_string__is_equal(&view, other)) {
        return true;
    }
    return false;
}

void
nk_string__rstrip(struct nk_string *self, const struct nk_string *other)
{
    while (nk_string__endswith(self, other)) {
        self->length -= other->length;
    }
}

void
nk_string__lower(struct nk_string *self)
{
    for (size_t i = 0u; i < self->length; i++) {
        self->array.items[i] = (char)tolower(self->array.items[i]);
    }
}

void
nk_string__upper(struct nk_string *self)
{
    for (size_t i = 0u; i < self->length; i++) {
        self->array.items[i] = (char)toupper(self->array.items[i]);
    }
}

char nk_char__lower(char character)
{
    return (char)tolower(character);
}

char nk_char__upper(char character)
{
    return (char)toupper(character);
}
