/*
 * nk_bits.c
 *
 *  Created on: May 25, 2021
 *      Author: nenad
 */

#include "eds/source/common/nk_bits.h"



uint32_t nk_bits__right_mask(uint_fast8_t index)
{
    static const uint32_t right_mask[32] =
    {
        [ 0] = 0x00000001u,
        [ 1] = 0x00000003u,
        [ 2] = 0x00000007u,
        [ 3] = 0x0000000fu,
        [ 4] = 0x0000001fu,
        [ 5] = 0x0000003fu,
        [ 6] = 0x0000007fu,
        [ 7] = 0x000000ffu,
        [ 8] = 0x000001ffu,
        [ 9] = 0x000003ffu,
        [10] = 0x000007ffu,
        [11] = 0x00000fffu,
        [12] = 0x00001fffu,
        [13] = 0x00003fffu,
        [14] = 0x00007fffu,
        [15] = 0x0000ffffu,
        [16] = 0x0001ffffu,
        [17] = 0x0003ffffu,
        [18] = 0x0007ffffu,
        [19] = 0x000fffffu,
        [20] = 0x001fffffu,
        [21] = 0x003fffffu,
        [22] = 0x007fffffu,
        [23] = 0x00ffffffu,
        [24] = 0x01ffffffu,
        [25] = 0x03ffffffu,
        [26] = 0x07ffffffu,
        [27] = 0x0fffffffu,
        [28] = 0x1fffffffu,
        [29] = 0x3fffffffu,
        [30] = 0x7fffffffu,
        [31] = 0xffffffffu
    };

    return right_mask[index & 0x1fu];
}
