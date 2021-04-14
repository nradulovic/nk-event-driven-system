/*
 * decoder.c
 *
 *  Created on: Apr 13, 2021
 *      Author: nenad
 */
#include <stdbool.h>
#include <stdint.h>

#include "generic/nk_array.h"
#include "generic/nk_farray.h"
#include "generic/nk_types.h"
#include "generic/nk_debug.h"
#include "generic/codec/nk_manchester.h"

struct encode_bit_scheme
{
    bool first;
    bool second;
};

struct encoding_scheme
    NK_FARRAY__T(struct encode_bit_scheme, 2)
;

struct decoding_scheme
    NK_FARRAY__T(uint_fast8_t, 4)
;

static void
encode_msb_first(const struct encoding_scheme *scheme, uint8_t value, struct nk_types__array__bool *encoded)
{
    for (size_t i = 0u; i < 8; i++) {
        bool bit_value = !!(value & (0x1u << (7u - i)));
        encoded->items[encoded->length++] = scheme->items[bit_value].first;
        encoded->items[encoded->length++] = scheme->items[bit_value].second;
    }
}

static enum nk_error
decode_msb_first(const struct decoding_scheme *scheme,
                 const struct nk_types__array__bool *source,
                 struct nk_types__array__u8 *decoded)
{
    decoded->items[decoded->length] = 0u;
    for (size_t i = 0u; i < source->length; i += 2u) {
        uint_fast8_t pair = (uint_fast8_t) ((source->items[i] << 0x1u) | source->items[i + 1u]);
        uint_fast8_t value = scheme->items[pair];

        if (value == UINT_FAST8_MAX) {
            return NK_ERROR__DATA_INVALID;
        }
        decoded->items[decoded->length] |= (uint8_t)((!!value) << (7u - (i / 2u)));
    }
    decoded->length++;
    return NK_ERROR__OK;
}

enum nk_error
nk_manchester__encode__biphasel(const struct nk_types__array__u8 *source, struct nk_types__array__bool *encoded)
{
    nk_assert(source);
    nk_assert(encoded);

    /*
     * 0 -> 0 | 1
     * 1 -> 1 | 0
     */
    static const struct encoding_scheme biphasel = { .items = { { .first = false, .second = true }, { .first = true,
                    .second = false } } };

    if ((source->length * 16u) > NK_ARRAY__FREE(encoded)) {
        return NK_ERROR__BUFFER_OVF;
    }
    for (size_t i = 0u; i < source->length; i++) {
        encode_msb_first(&biphasel, source->items[i], encoded);
    }
    return NK_ERROR__OK;
}

enum nk_error
nk_manchester__decode__biphasel(const struct nk_types__array__bool *source, struct nk_types__array__u8 *decoded)
{
    nk_assert(source);
    nk_assert(decoded);

    /*
     * 0 -> 0 | 1
     * 1 -> 1 | 0
     */
    static const struct decoding_scheme biphasel = { .items = { UINT_FAST8_MAX, 0, 1, UINT_FAST8_MAX } };

    if ((source->length % 2u) != 0u) {
        return NK_ERROR__DATA_ODD;
    }
    if (NK_BITS__DIVIDE_ROUNDUP((source->length / 2u), 8u) > NK_ARRAY__FREE(decoded)) {
        return NK_ERROR__BUFFER_OVF;
    }
    for (size_t i = 0u; i < source->length; i += 16u) {
        struct nk_types__array__bool window;
        NK_ARRAY__INITIALIZE_WINDOW(&window, source, i, i + 16u);
        enum nk_error error = decode_msb_first(&biphasel, &window, decoded);
        if (error) {
            return NK_ERROR__DATA_INVALID;
        }
    }
    return NK_ERROR__OK;
}
