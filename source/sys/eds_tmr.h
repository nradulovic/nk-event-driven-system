#ifndef NEON_KIT_EDS_SOURCE_EDS_TMR_H_
#define NEON_KIT_EDS_SOURCE_EDS_TMR_H_

#include <stdbool.h>

#include "eds_object.h"

void
eds_tmr__init(struct eds_object__tmr * self, void (* fn)(struct eds_object__tmr *));

#define eds_tmr__is_running(self)		((self)->state != EDS_OBJECT__TMR_STATE_DORMANT)

#define eds_tmr__is_periodic(self)		((self)->itime_ticks != 0u)

void
eds_tmr__start_after(struct eds_object__tmr * self, uint32_t after_ticks);

void
eds_tmr__start_every(struct eds_object__tmr * self, uint32_t every_ticks);

void
eds_tmr__cancel(struct eds_object__tmr * self);

void
eds_tmr__sentinel_init(void);

void
eds_tmr__sentinel_map_for_each_node(
    void (* map)(struct eds_object__tmr *, void *),
    void * arg);

#endif /* NEON_KIT_EDS_SOURCE_EDS_TMR_H_ */
