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
#define EDS_PORT__GLOBAL_CRITICAL           1
#else
#define EDS_PORT__GLOBAL_CRITICAL           0
#endif

struct eds_port__critical;

#if (EDS_PORT__GLOBAL_CRITICAL == 1)

extern struct eds_port__critical eds_port__global_critical;

#define EDS_PORT__CRITICAL_INSTANCE(name)	(void)0
#define EDS_PORT__CRITICAL_LOCK(name)		eds_port__critical_lock(&eds_port__global_critical)
#define EDS_PORT__CRITICAL_UNLOCK(name)		eds_port__critical_unlock(&eds_port__global_critical)
#else /* (EDS_PORT__GLOBAL_CRITICAL == 1) */
#define EDS_PORT__CRITICAL_INSTANCE(name)	struct eds_port__critical name
#define EDS_PORT__CRITICAL_LOCK(name)		eds_port__critical_lock(name)
#define EDS_PORT__CRITICAL_UNLOCK(name)		eds_port__critical_unlock(name)
#endif /* !(EDS_PORT__GLOBAL_CRITICAL == 1) */


void
eds_port__critical_lock(struct eds_port__critical * critical);
void
eds_port__critical_unlock(struct eds_port__critical * critical);

#if defined(EDS_PORT__USE_GLOBAL_SLEEP)
#define EDS_PORT__GLOBAL_SLEEP		        1
#else
#define EDS_PORT__GLOBAL_SLEEP		        0
#endif

#if (EDS_PORT__GLOBAL_SLEEP == 1)

extern struct eds_port__sleep eds_port__global_sleep;

#define EDS_PORT__SLEEP_INIT(instance)      eds_port__sleep_init(&eds_port__global_sleep)
#define EDS_PORT__SLEEP_WAIT(instance)      eds_port__sleep_wait(&eds_port__global_sleep)
#define EDS_PORT__SLEEP_SIGNAL(instance)    eds_port__sleep_signal(&eds_port__global_sleep)
#else /* (EDS_PORT__GLOBAL_SLEEP == 1) */
#define EDS_PORT__SLEEP_INIT(instance)      eds_port__sleep_init(instance)
#define EDS_PORT__SLEEP_WAIT(instance)      eds_port__sleep_wait(instance)
#define EDS_PORT__SLEEP_SIGNAL(instance)    eds_port__sleep_signal(instance)
#endif /* !(EDS_PORT__GLOBAL_SLEEP == 1) */

struct eds_port__sleep;

void
eds_port__sleep_init(struct eds_port__sleep * sleep);
void
eds_port__sleep_wait(struct eds_port__sleep * sleep);
void
eds_port__sleep_signal(struct eds_port__sleep * sleep);

#if !defined(EDS_PORT__USE_TICKLESS_TIMER)
#define EDS_PORT__USE_TICK_TIMER
#endif

#if defined(EDS_PORT__USE_GLOBAL_TIMER)
#define EDS_PORT__USE_LOCAL_TIMER           0
#else
#define EDS_PORT__USE_LOCAL_TIMER           1
#endif

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
