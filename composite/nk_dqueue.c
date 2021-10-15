/*
 * nk_dqueue.c
 *
 *  Created on: Oct 11, 2021
 *      Author: Nenad.Radulovic
 */

#include "generic/composite/nk_dqueue.h"
#include "generic/portable/nk_cpu.h"

void
nk_dqueue__init(struct nk_dqueue *self)
{
    self->segment_mark = 0u;
    for (size_t i = 0u; i < NK_FARRAY__LENGTH(&self->segments); i++) {
        nk_list__init(&self->segments.items[i]);
    }
}

void
nk_dqueue__insert(struct nk_dqueue *self,
                  struct nk_dqueue__node *node)
{
    nk_list__add_before(&self->segments.items[node->height], &node->node);
    self->segment_mark |= 0x1u << node->height;
}

struct nk_dqueue__node *
nk_dqueue__remove(struct nk_dqueue * self)
{
    struct nk_list * head;
    struct nk_list * entry;
    uint32_t i;

    i = nk_cpu__ffs(self->segment_mark);
    head = &self->segments.items[i];
    entry = nk_list__next(head);
    nk_list__remove(entry);
    if (nk_list__is_empty(head)) {
        self->segment_mark &= ~(0x1u << i);
    }
    return NK_BITS__CONTAINER_OF(entry, struct nk_dqueue__node, node);
}
