/*
 * decoder.c
 *
 *  Created on: Apr 13, 2021
 *      Author: nenad
 */
#include <stdint.h>

#include "../nk_farray.h"
#include "../nk_debug.h"

#include "codec/manchester.h"

struct coding_scheme
    NK_FARRAY__T(bool, 4)
;

static void encode_lsb_first(const struct coding_scheme *scheme,
                             uint8_t value,
                             struct nk_varray__bool *encoded)
{
    for (size_t i = 0u; i < 8; i++) {
        bool bit_value = value & 0x1u;
        encoded->array.items[i] = scheme->array[bit_value];
    }
}

enum nk_error mde__encode__biphasel(const struct nk_varray__u8 *source,
                                    struct nk_varray__bool *encoded)
{
    nk_assert(source); nk_assert(encoded);

    static const struct coding_scheme biphasel =
            NK_FARRAY__INITIALIZE_WITH(&biphasel, false, true, true, false);

    if ((source->length * 16u) > encoded->length) {
        return NK_ERROR__BUFFER_OVF;
    }
    for (size_t i = 0u; i < source->length; i++) {
        struct nk_varray__bool window;
        NK_VARRAY__INITIALIZE_WINDOW(&window, encoded, i * 8u, (i + 1u) * 8u);
        encode_lsb_first(&biphasel, source->array.items[i], &window);
    }
}
