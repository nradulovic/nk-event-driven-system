/*
 * eds_etimer.c
 *
 *  Created on: Dec 9, 2021
 *      Author: nenad
 */

#include "sys/eds_etm.h"
#include "sys/eds_epa.h"
#include "sys/eds_evt.h"
#include "sys/eds_mem.h"
#include "sys/eds_tmr.h"
#include "sys/eds_core.h"
#include "sys/eds_state.h"
#include "eds.h"
#include "eds_port.h"
#include "eds_trace.h"

static eds__error
etm_create(
    eds__sm * sm, 
    uint32_t event_id,  
    const struct eds__etimer_attr * attr, 
    eds__etimer ** etimer);
static eds__error
etm_delete(eds__etimer * etimer);
static eds__error
etm_cancel(eds__etimer * etimer);
static eds__error
etm_start_after(eds__etimer * etimer, uint32_t after_ms);
static eds__error
etm_start_every(eds__etimer * etimer, uint32_t after_ms);
static void
etm_match_and_cancel_delete(struct eds_object__tmr * tmr_node, void * arg);
static void
etm_cb(struct eds_object__tmr * tmr);

static eds__error
etm_create(
    eds__sm * sm, 
    uint32_t event_id,  
    const struct eds__etimer_attr * attr, 
    eds__etimer ** etm)
{
    struct eds_object__mem * mem;
    struct eds_object__etm * l_etm;

    if (attr->static_instance == NULL) {
        mem = eds_mem__find(&eds_state__mem_instances, sizeof(*l_etm));
        if (mem == NULL) {
            EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CREATE, EDS__ERROR_NO_RESOURCE, "%u", sizeof(*l_etm));
            return EDS__ERROR_NO_RESOURCE;
        }
        eds_port__critical_lock();
        l_etm = eds_mem__allocate_from(mem, sizeof(*l_etm));
        eds_port__critical_unlock();
        if (l_etm == NULL) {
            EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CREATE, EDS__ERROR_NO_MEMORY, "%u from %p", sizeof(*l_etm), mem);
            return EDS__ERROR_NO_MEMORY;
        }
    } else {
        mem = NULL;
        l_etm = attr->static_instance;
    }
    eds_tmr__init(&l_etm->p__node, &etm_cb);
    eds_evt__init(&l_etm->p__evt, event_id, 0, NULL);
    l_etm->p__epa = eds__agent_from_sm(sm);
    l_etm->p__mem = mem;
    *etm = l_etm;
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CREATE, EDS__ERROR_NONE, "id = (%p)", l_etm);
    return EDS__ERROR_NONE;
}

static eds__error
etm_delete(eds__etimer * etimer)
{
    if (etimer->p__mem != NULL) {
    	eds_port__critical_lock();
        eds_mem__deallocate_to(etimer->p__mem, etimer);
        eds_port__critical_unlock();
    }
    return EDS__ERROR_NONE;
}

static eds__error
etm_cancel(eds__etimer * etimer)
{
    /* To have designated timer but no designated EPA cannot happen. */
    if (eds_tmr__is_running(&etimer->p__node) == false) {
        return EDS__ERROR_NOT_EXISTS;
    }
    eds_tmr__cancel(&etimer->p__node);
    eds_evt__null(&etimer->p__evt); /* We want to null-ify this event for all receivers */
    return EDS__ERROR_NONE;
}

static eds__error
etm_start_after(eds__etimer * etimer, uint32_t after_ms)
{
    if (eds_tmr__is_running(&etimer->p__node) == true) {
        return EDS__ERROR_ALREADY_EXISTS;
    }
    eds_tmr__start_after(&etimer->p__node, eds_port__tick_from_ms(after_ms));
    return EDS__ERROR_NONE;
}

static eds__error
etm_start_every(eds__etimer * etimer, uint32_t after_ms)
{
    if (eds_tmr__is_running(&etimer->p__node) == true) {
        return EDS__ERROR_ALREADY_EXISTS;
    }
    eds_tmr__start_every(&etimer->p__node, eds_port__tick_from_ms(after_ms));
    return EDS__ERROR_NONE;
}

static void
etm_match_and_cancel_delete(struct eds_object__tmr * tmr_node, void * arg)
{
    struct eds_object__epa * epa = arg;
    struct eds_object__etm * etm;

    etm = EDS_CORE__CONTAINER_OF(tmr_node, struct eds_object__etm, p__node);
    if (epa == etm->p__epa) {
        etm_cancel(etm);
        etm->p__epa = NULL; /* Designate that the timer is not owned by any agent */
        eds_evt__null(&etm->p__evt); /* We want to null-ify this event for all receivers */
        if (etm->p__mem != NULL) {
            eds_mem__deallocate_to(etm->p__mem, etm);
        }
    }
}

static void
etm_cb(struct eds_object__tmr * tmr)
{
    struct eds_object__etm * etm;

    etm = EDS_CORE__CONTAINER_OF(tmr, struct eds_object__etm, p__node);
    // TODO: Error handling when EPA queue is full
    eds_epa__send(etm->p__epa, &etm->p__evt);
}

void
eds_etm_service__delete_all(struct eds_object__epa * epa)
{
    eds_tmr__sentinel_map_for_each_node(etm_match_and_cancel_delete, epa);
}

void
eds_etm_service__init(void)
{
    eds_tmr__sentinel_init();
}

eds__error
eds__etimer_create(
    eds__sm * sm, 
    uint32_t event_id,  
    const struct eds__etimer_attr * attr, 
    eds__etimer ** etimer)
{
    static const struct eds__etimer_attr default_attr;

    if ((sm == NULL ) || (etimer == NULL)) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CREATE, EDS__ERROR_INVALID_ARGUMENT, "attr, etimer = (%p, %p)", attr, etimer);
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (attr == NULL) {
        attr = &default_attr;
    }
    return etm_create(sm, event_id, attr, etimer);
}

eds__error
eds__etimer_delete(eds__etimer * etimer)
{
    if (etimer == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    return etm_delete(etimer);
}

eds__error
eds__etimer_send_after(eds__etimer * etimer, uint32_t after_ms)
{
    eds__error error;

    if (etimer == NULL) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_INVALID_ARGUMENT, "etimer, agent = (%p, %p)", etimer, agent);
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_port__tick_from_ms(after_ms) == 0u) {
        return EDS__ERROR_OUT_OF_RANGE;
    }
    eds_port__critical_lock();
    error = etm_start_after(etimer, after_ms);
    eds_port__critical_unlock();
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_NONE, "etimer, agent, event_id, after_ms = (%p, %p, %u, %u)", etimer, agent, event_id, after_ms);
    return error;
}

eds__error
eds__etimer_send_every(eds__etimer * etimer, uint32_t every_ms)
{
    eds__error error;

    if (etimer == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_port__tick_from_ms(every_ms) == 0u) {
        return EDS__ERROR_OUT_OF_RANGE;
    }
    eds_port__critical_lock();
    error = etm_start_every(etimer, every_ms);
    eds_port__critical_unlock();
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_NONE, "etimer, agent, event_id, every_ms = (%p, %p, %u, %u)", etimer, agent, event_id, every_ms);
    return error;
}

eds__error
eds__etimer_cancel(eds__etimer * etimer)
{
    eds__error error;

    if (etimer == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    eds_port__critical_lock();
    error = etm_cancel(etimer);
    eds_port__critical_unlock();
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CANCEL, error, "etimer = (%p)", etimer);
    return error;
}
