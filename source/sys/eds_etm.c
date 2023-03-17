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

#define etm_start_every(etm, ticks)         eds_tmr__start_every(&(etm)->p__node, (ticks))
#define etm_is_periodic(etm)                eds_tmr__is_periodic(&(etm)->p__node)
#define etm_is_designated(etm)              ((etm)->p__epa != NULL)

static eds__error
etm_create(const struct eds__etimer_attr * attr, eds__etimer ** etm);
static eds__error
etm_delete(eds__etimer * etimer);
static eds__error
etm_cancel(eds__etimer * etimer);
static eds__error
etm_start_after(eds__etimer * etimer,
    eds__agent * agent,
    const eds__event * event,
    uint32_t after_ms);
static void
etm_match_and_cancel_delete(struct eds_object__tmr * tmr_node, void * arg);
static void
etm_cb(struct eds_object__tmr * tmr);

static eds__error
etm_create(const struct eds__etimer_attr * attr, eds__etimer ** etm)
{
    struct eds_object__mem * mem;
    struct eds_object__etm * l_etm;

    if (attr->static_instance == NULL) {
	EDS_PORT__CRITICAL_INSTANCE(critical);

        mem = eds_mem__find(&eds_state__mem_instances, sizeof(*l_etm));
        if (mem == NULL) {
            EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CREATE, EDS__ERROR_NO_RESOURCE, "%u", sizeof(*l_etm));
            return EDS__ERROR_NO_RESOURCE;
        }
        EDS_PORT__CRITICAL_LOCK(&critical);
        l_etm = eds_mem__allocate_from(mem, sizeof(*l_etm));
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        if (l_etm == NULL) {
            EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CREATE, EDS__ERROR_NO_MEMORY, "%u from %p", sizeof(*l_etm), mem);
            return EDS__ERROR_NO_MEMORY;
        }
    } else {
        mem = NULL;
        l_etm = attr->static_instance;
    }
    eds_tmr__init(&l_etm->p__node, &etm_cb);
    l_etm->p__evt = NULL; /* No associated event with this timer */
    l_etm->p__epa = NULL; /* No associated agent with this timer */
    l_etm->p__mem = mem;
    *etm = l_etm;
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CREATE, EDS__ERROR_NONE, "id = (%p)", l_etm);
    return EDS__ERROR_NONE;
}

static eds__error
etm_delete(eds__etimer * etimer)
{
	EDS_PORT__CRITICAL_INSTANCE(critical);

    if (etimer->p__mem != NULL) {
        EDS_PORT__CRITICAL_LOCK(&critical);
        eds_mem__deallocate_to(etimer->p__mem, etimer);
        EDS_PORT__CRITICAL_UNLOCK(&critical);
    }
    return EDS__ERROR_NONE;
}

static eds__error
etm_cancel(eds__etimer * etimer)
{
    /* To have designated timer but no designated EPA cannot happen. */
    eds_tmr__cancel(&etimer->p__node);
    eds_evt__null(etimer->p__evt); /* We want to null-ify this event for all receivers */
    eds_evt__ref_down(etimer->p__evt);
    eds_evt__deallocate(etimer->p__evt); /* Dispose of event as well */
    etimer->p__evt = NULL;
    etimer->p__epa = NULL; /* Designate that the timer is not owned by any agent */
    return EDS__ERROR_NONE;
}

static eds__error
etm_start_after(eds__etimer * etimer,
    eds__agent * agent,
    const eds__event * event,
    uint32_t after_ms)
{
    if (eds_epa__is_designated(agent) == false) {
        return EDS__ERROR_NO_PERMISSION;
    }
    if (etm_is_designated(etimer) == true) {
        return EDS__ERROR_ALREADY_EXISTS;
    }
    etimer->p__epa = agent; /* Designate that this timer is owned by the agent */
    etimer->p__evt = event;
    eds_evt__ref_up(event);
    eds_tmr__start_after(&etimer->p__node, eds_port__tick_from_ms(after_ms));
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
        eds_evt__null(etm->p__evt); /* We want to null-ify this event for all receivers */
        eds_evt__ref_down(etm->p__evt);
        eds_evt__deallocate(etm->p__evt); /* Dispose of event as well */
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
    eds_epa__send(etm->p__epa, etm->p__evt);
    if (etm_is_periodic(etm) == false) {
        etm->p__epa = NULL;
    }
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
eds__etimer_create(const struct eds__etimer_attr * attr, eds__etimer ** etimer)
{
    static const struct eds__etimer_attr default_attr;

    if (etimer == NULL) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CREATE, EDS__ERROR_INVALID_ARGUMENT, "attr, etimer = (%p, %p)", attr, etimer);
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (attr == NULL) {
        attr = &default_attr;
    }
    eds_state__has_started = true;
    return etm_create(attr, etimer);
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
eds__etimer_send_after(eds__etimer * etimer,
    eds__agent * agent,
    const eds__event * event,
    uint32_t after_ms)
{
	EDS_PORT__CRITICAL_INSTANCE(critical);
    eds__error error;

    if ((etimer == NULL) || (agent == NULL) || (event == NULL)) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_INVALID_ARGUMENT, "etimer, agent, event = (%p, %p, %p)", etimer, agent, event);
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_evt__is_dynamic(event) == false) {
        return EDS__ERROR_NO_RESOURCE;
    }
    if (after_ms < eds_port__tick_duration_ms()) {
        return EDS__ERROR_OUT_OF_RANGE;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    error = etm_start_after(etimer, agent, event, after_ms);
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_NONE, "etimer, agent, event_id, after_ms = (%p, %p, %u, %u)", etimer, agent, event->p__id, after_ms);
    return error;
}

eds__error
eds__etimer_send_every(eds__etimer * etimer,
    eds__agent * agent,
    const eds__event * event,
    uint32_t every_ms)
{
	EDS_PORT__CRITICAL_INSTANCE(critical);

    if ((etimer == NULL) || (agent == NULL) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_evt__is_dynamic(event) == false) {
        return EDS__ERROR_NO_RESOURCE;
    }
    if (every_ms < eds_port__tick_duration_ms()) {
        return EDS__ERROR_OUT_OF_RANGE;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    if (eds_epa__is_designated(agent) == false) {
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        return EDS__ERROR_NO_PERMISSION;
    }
    if (etm_is_designated(etimer) == true) {
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        return EDS__ERROR_ALREADY_EXISTS;
    }
    etimer->p__epa = agent; /* Designate that this timer is owned by the agent */
    etimer->p__evt = event;
    eds_evt__ref_up(event);
    etm_start_every(etimer, eds_port__tick_from_ms(every_ms));
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__etimer_cancel(eds__etimer * etimer)
{
	EDS_PORT__CRITICAL_INSTANCE(critical);
    eds__error error;

    if (etimer == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    if (etm_is_designated(etimer) == false) {
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        return EDS__ERROR_NOT_EXISTS;
    }
    error = etm_cancel(etimer);
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CANCEL, error, "etimer = (%p)", etimer);
    return error;
}
