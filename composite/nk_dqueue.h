/*
 * nk_dqueue.h
 *
 *  Created on: Oct 11, 2021
 *      Author: Nenad.Radulovic
 */

#ifndef GENERIC_COMPOSITE_NK_DQUEUE_H_
#define GENERIC_COMPOSITE_NK_DQUEUE_H_

#include <stdint.h>

#include "generic/common/nk_bits.h"
#include "generic/composite/nk_farray.h"
#include "generic/composite/nk_list.h"

struct nk_dqueue
{
    uint32_t segment_mark;
    struct nk_dqueue__segments
        NK_FARRAY__T(struct nk_list, NK_BITS__BIT_SIZE(uint32_t))
    segments;
};

struct nk_dqueue__node
{
    struct nk_list node;
    uint_fast8_t height;
};

void
nk_dqueue__init(struct nk_dqueue *selft);

void
nk_dqueue__insert(struct nk_dqueue *self,
                  struct nk_dqueue__node *node);

struct nk_dqueue__node*
nk_dqueue__remove(struct nk_dqueue *self);

#endif /* GENERIC_COMPOSITE_NK_DQUEUE_H_ */
