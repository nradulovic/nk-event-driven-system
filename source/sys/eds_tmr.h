#ifndef NEON_KIT_EDS_SOURCE_EDS_TMR_H_
#define NEON_KIT_EDS_SOURCE_EDS_TMR_H_

#include <stdbool.h>

#include "eds_object.h"

void
eds_tmr__node_init(struct eds_object__tmr_node * self, void (* fn)(struct eds_object__tmr_node *));

bool
eds_tmr__node_is_running(const struct eds_object__tmr_node * self);

bool
eds_tmr__node_is_periodic(const struct eds_object__tmr_node * self);

void
eds_tmr__start_after(
    struct eds_object__tmr_sentinel * self,
    struct eds_object__tmr_node * node,
    uint32_t after_ticks);

void
eds_tmr__start_every(
    struct eds_object__tmr_sentinel * self,
    struct eds_object__tmr_node * node,
    uint32_t every_ticks);

void
eds_tmr__cancel(struct eds_object__tmr_sentinel * self, struct eds_object__tmr_node * node);

void
eds_tmr__init(struct eds_object__tmr_sentinel * self);

bool
eds_tmr__are_timers_pending(const struct eds_object__tmr_sentinel * self);

void
eds_tmr__for_each_node(
    struct eds_object__tmr_sentinel * self,
    void (* map)(struct eds_object__tmr_node *, void *),
    void * arg);

void
eds_tmr__process_timers(struct eds_object__tmr_sentinel * self);

#endif /* NEON_KIT_EDS_SOURCE_EDS_TMR_H_ */
