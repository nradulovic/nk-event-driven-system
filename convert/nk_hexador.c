/*
 * nk_hexador.c
 *
 *  Created on: Apr 12, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 *  08/05/2021: (nbr) Minor code formatting
 *  13/07/2021: (nbr) Not using nk_types since they were moved to nk_array
 */

#include "generic/common/nk_debug.h"
#include "generic/composite/nk_string.h"
#include "generic/convert/nk_hexador.h"
#include "generic/convert/nk_convert.h"


struct nk_result__size
nk_hexador__to_bin(const struct nk_string *string,
                   struct nk_types__array__u8 *buffer)
{
    struct nk_result__size result;

    nk_assert(string);
    nk_assert(buffer);

    if ((string->length % 2u) != 0u) {
        result.error = NK_ERROR__DATA_ODD;
        result.value = 0u;
        return result;
    }
    if ((string->length / 2u) > buffer->item_no) {
        result.error = NK_ERROR__BUFFER_OVF;
        result.value = 0u;
        return result;
    }
    result.error = NK_ERROR__OK;
    result.value = string->length;
    for (size_t i = 0u; i < string->length; i += 2u) {
        struct nk_result__u8 htob_result;
        struct nk_convert__hex hex;

        hex.msb = string->items[i];
        hex.lsb = string->items[i + 1];
        htob_result = nk_convert__hex_to_bin(hex);
        if (htob_result.error != NK_ERROR__OK) {
            result.error = NK_ERROR__DATA_INVALID;
            result.value = i;
            break;
        }
        buffer->items[i / 2u] = htob_result.value;
        buffer->length++;
    }
    return result;
}

struct nk_result__size
nk_hexador__to_hex(const struct nk_types__array__u8 *buffer,
                   struct nk_string *string)
{
    struct nk_result__size result;

    nk_assert(string);
    nk_assert(buffer);

    if (string->item_no < (buffer->length * 2u)) {
        result.error = NK_ERROR__BUFFER_OVF;
        result.value = 0u;
        return result;
    }

    for (size_t i = 0u; i < buffer->length; i++) {
        struct nk_convert__hex btoh_result = nk_convert__bin_to_hex(buffer->items[i]);
        string->items[string->length++] = btoh_result.msb;
        string->items[string->length++] = btoh_result.lsb;
    }
    result.error = NK_ERROR__OK;
    result.value = buffer->length;
    return result;
}

