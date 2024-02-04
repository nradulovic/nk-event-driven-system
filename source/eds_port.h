/*
 * nk_eds_port.h
 *
 *  Created on: Dec 2, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_NK_EDS_PORT_H_
#define NEON_KIT_GENERIC_SOURCE_NK_EDS_PORT_H_

#include <stdint.h>
#include <stddef.h>

#include "eds_port/eds_port_definition.h"

#if !defined(EDS_PORT__USE_GLOBAL_SLEEP) || \
	((EDS_PORT__USE_GLOBAL_SLEEP != 0) && (EDS_PORT__USE_GLOBAL_SLEEP != 1))
#error "EDS port definition (eds_port_definition.h) needs to define EDS_PORT__USE_GLOBAL_SLEEP to 0 or 1"
#endif

void
eds_port__critical_lock(void);
void
eds_port__critical_unlock(void);

struct eds_port__sleep;

void
eds_port__sleep_init(struct eds_port__sleep * sleep);
void
eds_port__sleep_wait(struct eds_port__sleep * sleep);
void
eds_port__sleep_signal(struct eds_port__sleep * sleep);

#if (EDS_PORT__USE_GLOBAL_SLEEP == 1)

extern struct eds_port__sleep eds_port__global_sleep;

#endif /* !(EDS_PORT__USE_GLOBAL_SLEEP == 0) */

void
eds_port__timer_set_cb(void (* callback)(void));

void
eds_port__timer_start(void);

void
eds_port__timer_stop(void);

uint_fast8_t
eds_port__ffs(uint32_t value);

#ifndef EDS_PORT__ALIGN_UP
#define EDS_PORT__ALIGN_UP(size) \
    (((size) + sizeof(void *) - 1u) & ~(sizeof(void *) - 1u))
#endif

uint32_t
eds_port__tick_from_ms(uint32_t ms);

void
eds_port__init(void);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_PORT_H_ */
