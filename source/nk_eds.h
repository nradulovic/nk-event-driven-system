/*
 * nk_eds.h
 *
 *  Created on: Dec 2, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_NK_EDS_H_
#define NEON_KIT_GENERIC_SOURCE_NK_EDS_H_

#include <stdint.h>
#include <stddef.h>

typedef struct eds_object__epa eds_epa;
typedef struct eds_object__sm eds_sm;
typedef struct eds_object__event eds_event;
typedef uint32_t eds_error;

eds_error
eds_event__add_mem(void* (*alloc)(size_t size),
                   void  (*dealloc)(void*),
                   size_t max_size);

eds_error
eds_event__create(uint32_t event_id, size_t event_data_size, eds_event ** event);

uint32_t eds_event__id(const eds_event * event);
void * eds_event__data(eds_event * event);

typedef enum eds_sm__action
{
    EDS_SM__ACTION__SUPER,
    EDS_SM__ACTION__HANDLED,
    EDS_SM__ACTION__IGNORED,
    EDS_SM__ACTION__PUSHED_BACK,
    EDS_SM__ACTION__TRANSIT
} eds_sm__action;

typedef eds_sm__action
(eds_sm__state_fn)(eds_sm*, void *workspace, const eds_event *event);

typedef uint32_t eds_sm__prio;

struct eds_sm__attr
{
    const char *name;
    eds_sm__prio prio;
    eds_sm *instance;
    size_t equeue_size;
    eds_event *equeue_storage;
};

eds_error
eds_sm__create(eds_sm__state_fn *initial_state, void *sm_workspace, const struct eds_sm__attr *attr, eds_sm **sm);

eds_error
eds_sm__delete(eds_sm *sm);

eds_error
eds_sm__send_signal(eds_sm * sm, uint32_t event_id, uint32_t timeout_ms);

eds_error
eds_sm__send_event(eds_sm *sm, const eds_event *event, uint32_t timeout_ms);

eds_error
eds_sm__send_event_after(eds_sm *sm, const eds_event *event, uint32_t after_ms);

eds_error
eds_sm__send_event_every(eds_sm *sm, const eds_event *event, uint32_t every_ms);

eds_epa*
eds_sm__get_epa(const eds_sm *sm);

struct eds_epa__attr
{
    const char *name;
    eds_epa *instance;
};

eds_error
eds_epa__create(const eds_epa__attr *atrr, eds_epa **epa);

eds_error
eds_epa__delete(eds_epa *epa);

eds_error
eds_epa__add(eds_epa *epa, eds_sm *sm);

eds_error
eds_epa__remove(eds_epa *epa, eds_sm *sm);

eds_error
eds_epa__start_all(eds_epa *epa);

eds_error
eds_epa__stop_all(eds_epa *epa);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_H_ */
