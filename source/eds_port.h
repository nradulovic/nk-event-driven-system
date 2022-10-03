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

#if defined(EDS_PORT__USE_GLOBAL_CRITICAL)
#define EDS_PORT__USE_LOCAL_CRITICAL		0
#define EDS_PORT__CRITICAL_INSTANCE(name)	(void)0
#define EDS_PORT__CRITICAL_LOCK(name)		eds_port__critical_global_lock()
#define EDS_PORT__CRITICAL_UNLOCK(name)		eds_port__critical_global_unlock()
#else /* defined(EDS_PORT__USE_GLOBAL_CRITICAL) */
#define EDS_PORT__USE_LOCAL_CRITICAL		1
#define EDS_PORT__CRITICAL_INSTANCE(name)	struct eds_port__critical_local name
#define EDS_PORT__CRITICAL_LOCK(name)		eds_port__critical_local_lock(name)
#define EDS_PORT__CRITICAL_UNLOCK(name)		eds_port__critical_local_unlock(name)
#endif /* !defined(EDS_PORT__USE_GLOBAL_CRITICAL) */

void
eds_port__critical_global_lock(void);
void
eds_port__critical_global_unlock(void);

struct eds_port__critical_local;

void
eds_port__critical_local_lock(struct eds_port__critical_local * critical);
void
eds_port__critical_local_unlock(struct eds_port__critical_local * critical);

#if defined(EDS_PORT__USE_GLOBAL_SLEEP)
#define EDS_PORT__USE_LOCAL_SLEEP		0
#else
#define EDS_PORT__USE_LOCAL_SLEEP		1
#endif

void
eds_port__sleep_global_init(void);
void
eds_port__sleep_global_wait(void);
void
eds_port__sleep_global_signal(void);

struct eds_port__sleep_local;

void
eds_port__sleep_local_init(struct eds_port__sleep_local * sleep);
void
eds_port__sleep_local_wait(struct eds_port__sleep_local * sleep);
void
eds_port__sleep_local_signal(struct eds_port__sleep_local * sleep);

#if !defined(EDS_PORT__USE_TICKLESS_TIMER)
#define EDS_PORT__USE_TICK_TIMER
#endif

struct eds_port__timer;

void
eds_port__timer_init(struct eds_port__timer * timer);

void
eds_port__timer_start(struct eds_port__timer * timer, uint32_t ms);

void
eds_port__timer_stop(struct eds_port__timer * timer);

uint_fast8_t
eds_port__ffs(uint32_t value);

size_t
eds_port__align_up(size_t non_aligned_value);

uint32_t
eds_port__tick_duration_ms(void);

uint32_t
eds_port__tick_from_ms(uint32_t ms);

void
eds_port__init(void);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_PORT_H_ */
