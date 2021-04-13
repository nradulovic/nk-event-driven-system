/*
 * nk_convert.c
 *
 *  Created on: Apr 12, 2021
 *      Author: nenad
 */

#include "nk_array.h"
#include "nk_convert.h"
#include "nk_string.h"

struct hex_map_to_int
{
    char from;
    uint8_t to;
};

struct nk_convert__hex_to_bin__result nk_convert__hex_to_bin(char character)
{
    struct nk_convert__hex_to_bin__result result;

    static const struct hex_map_to_int_array
        NK_ARRAY__BUCKET_T(const struct hex_map_to_int, 16u)
    hex_map =
        NK_ARRAY__BUCKET_INITIALIZER_WITH(&hex_map,
                        {
                            '0',
                            0},
                        {
                            '1',
                            1},
                        {
                            '2',
                            2},
                        {
                            '3',
                            3},
                        {
                            '4',
                            4},
                        {
                            '5',
                            5},
                        {
                            '6',
                            6},
                        {
                            '7',
                            7},
                        {
                            '8',
                            8},
                        {
                            '9',
                            9},
                        {
                            'a',
                            10},
                        {
                            'b',
                            11},
                        {
                            'c',
                            12},
                        {
                            'd',
                            13},
                        {
                            'e',
                            14},
                        {
                            'f',
                            15})
    ;

    result.error = NK_ERROR__DATA_INVALID;
    result.value = 0u;

    for (size_t i = 0u; i < hex_map.array.item_no; i++) {
        if (hex_map.array.items[i].from == nk_char__lower(character)) {
            result.error = NK_ERROR__OK;
            result.value = hex_map.array.items[i].to;
            break;
        }
    }
    return result;
}

struct nk_convert__bin_to_hex__result nk_convert__bin_to_hex(uint8_t byte)
{
    struct nk_convert__bin_to_hex__result result;

    static const struct bin_map_to_char_array
        NK_ARRAY__BUCKET_T(const uint8_t, 16)
    bin_map =
        NK_ARRAY__BUCKET_INITIALIZER_WITH(&bin_map, '0',
                        '1',
                        '2',
                        '3',
                        '4',
                        '5',
                        '6',
                        '7',
                        '8',
                        '9',
                        'a',
                        'b',
                        'c',
                        'd',
                        'e',
                        'f')
    ;
    if (byte > bin_map.array.item_no) {
        result.error = NK_ERROR__DATA_OVF;
        result.value = 0u;
    } else {
        result.error = NK_ERROR__OK;
        result.value = bin_map.array.items[byte];
    }
    return result;
}
