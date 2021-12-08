
#ifndef NEON_KIT_EDS_SOURCE_EDS_TMR_H_
#define NEON_KIT_EDS_SOURCE_EDS_TMR_H_

#include "eds_object.h"
#include <stdbool.h>

void
eds_tmr__init(struct eds_object__tmr * tmr, void (*fn)(struct eds_object__tmr*));

void
eds_tmr__start_once(struct eds_object__tmr *tmr, uint32_t tick);

void
eds_tmr__start_periodic(struct eds_object__tmr *tmr, uint32_t ticks);

void
eds_tmr__cancel(struct eds_object__tmr *tmr);

bool
eds_tmr__is_running(const struct eds_object__tmr *tmr);

bool
eds_tmr__are_timers_pending(void);

void
eds_tmr__process_timers(void);

#endif /* NEON_KIT_EDS_SOURCE_EDS_TMR_H_ */