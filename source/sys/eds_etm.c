/*
 * eds_etimer.c
 *
 *  Created on: Dec 9, 2021
 *      Author: nenad
 */

#include "sys/eds_etm.h"
#include "eds.h"
#include "sys/eds_epa.h"
#include "sys/eds_epn.h"
#include "sys/eds_evt.h"
#include "sys/eds_mem.h"
#include "sys/eds_tmr.h"
#include "sys/eds_core.h"
#include "sys/eds_state.h"
#include "eds_port.h"
#include "eds_trace.h"

struct tuple_service_epa
{
    struct eds_object__epa * epa;
    struct eds_object__etm_sentinel * etm_service;
};

static void
etm__every_handler(struct eds_object__tmr_node * tmr)
{
    struct eds_object__etm_node * etm;

    etm = EDS_CORE__CONTAINER_OF(tmr, struct eds_object__etm_node, p__node);
    // TODO: Error handling when EPA queue is full
    if (eds_epa__is_designated(etm->p__epa) == true) {
        eds_epa__send(etm->p__epa, etm->p__evt);
        if (!eds_tmr__node_is_periodic(tmr)) {
            eds_etm__designate(etm, NULL);
        }
    }
}

static void
match_and_cancel_delete(struct eds_object__tmr_node * tmr_node, void * arg)
{
    struct tuple_service_epa * tuple_service_epa = arg;
    struct eds_object__etm_node * etm;

    etm = EDS_CORE__CONTAINER_OF(tmr_node, struct eds_object__etm_node, p__node);
    if (tuple_service_epa->epa == etm->p__epa) {
        eds_tmr__cancel(&tuple_service_epa->etm_service->p__tmr, &etm->p__node);
        eds_etm__designate(etm, NULL); /* Designate that the timer is not owned by any agent */
        eds_evt__null(etm->p__evt); /* We want to null-ify this event for all receivers */
        eds_evt__ref_down(etm->p__evt);
        eds_evt__deallocate(etm->p__evt); /* Dispose of event as well */
        if (etm->p__mem != NULL) {
            eds_mem__deallocate_to(etm->p__mem, etm);
        }
    }
}

extern inline void
eds_etm__designate(struct eds_object__etm_node * etm, struct eds_object__epa * epa);

extern inline bool
eds_etm__is_designated(const struct eds_object__etm_node * etm);

extern inline void
eds_etm__set_event(struct eds_object__etm_node * etm, const struct eds_object__evt * evt);

eds__error
eds_etm__create(const struct eds__etimer_attr * attr, eds__etimer ** etm)
{
    struct eds_object__mem * mem;
    struct eds_object__etm_node * l_etm;

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
    eds_tmr__node_init(&l_etm->p__node, &etm__every_handler);
    l_etm->p__evt = NULL; /* No associated event with this timer */
    l_etm->p__epa = NULL; /* No associated agent with this timer */
    l_etm->p__mem = mem;
    *etm = l_etm;
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CREATE, EDS__ERROR_NONE, "id = (%p)", l_etm);
    return EDS__ERROR_NONE;
}

eds__error
eds_etm__delete(eds__etimer * etimer)
{
	EDS_PORT__CRITICAL_INSTANCE(critical);

    if (etimer->p__mem != NULL) {
        EDS_PORT__CRITICAL_LOCK(&critical);
        eds_mem__deallocate_to(etimer->p__mem, etimer);
        EDS_PORT__CRITICAL_UNLOCK(&critical);
    }
    return EDS__ERROR_NONE;
}

void
eds_etm_service__delete_all(struct eds_object__etm_sentinel * etm_service, struct eds_object__epa * epa)
{
    struct tuple_service_epa tuple_service_epa =
    {
        .epa = epa, 
        .etm_service = etm_service
    };
    eds_tmr__for_each_node(&etm_service->p__tmr, match_and_cancel_delete, &tuple_service_epa);
}

void
eds_etm_service__init(struct eds_object__etm_sentinel * etm_service)
{
    eds_tmr__init(&etm_service->p__tmr);
}

void
eds_etm_service__start_once(struct eds_object__etm_sentinel * etm_service,
    struct eds_object__etm_node * etm,
    uint32_t ms)
{
    eds_tmr__start_after(&etm_service->p__tmr, &etm->p__node, ms);
}

void
eds_etm_service__start_periodic(struct eds_object__etm_sentinel * etm_service,
    struct eds_object__etm_node * etm,
    uint32_t ms)
{
    eds_tmr__start_every(&etm_service->p__tmr, &etm->p__node, ms);
}

void
eds_etm_service__cancel(struct eds_object__etm_sentinel * etm_service, struct eds_object__etm_node * etm)
{
    eds_tmr__cancel(&etm_service->p__tmr, &etm->p__node);
}

void
eds_etm_service__tick(struct eds_object__etm_sentinel * etm_service)
{
    eds_tmr__process_timers(&etm_service->p__tmr);
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
    return eds_etm__create(attr, etimer);
}

eds__error
eds__etimer_delete(eds__etimer * etimer)
{
    if (etimer == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    return eds_etm__delete(etimer);
}

eds__error
eds__etimer_send_after(eds__etimer * etimer,
    eds__agent * agent,
    const eds__event * event,
    uint32_t after_ms)
{
    struct eds_object__epn * epn;
	EDS_PORT__CRITICAL_INSTANCE(critical);

    if ((etimer == NULL) || (agent == NULL) || (event == NULL)) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_INVALID_ARGUMENT, "etimer, agent, event = (%p, %p, %p)", etimer, agent, event);
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (after_ms < eds_port__tick_duration_ms()) {
        return eds__agent_send(agent, event);
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    if (eds_epa__is_designated(agent) == false) {
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_NO_PERMISSION, "etimer, agent, event = (%p, %p, %p)", etimer, agent, event);
        return EDS__ERROR_NO_PERMISSION;
    }
    if (eds_etm__is_designated(etimer) == true) {
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_ALREADY_EXISTS, "etimer, agent, event = (%p, %p, %p)", etimer, agent, event);
        return EDS__ERROR_ALREADY_EXISTS;
    }

    epn = eds_epa__designation(agent);
    eds_etm__designate(etimer, agent); /* Designate that this timer is owned by the agent */
    eds_etm__set_event(etimer, event);
    eds_evt__ref_up(event);
    eds_etm_service__start_once(eds_epn__etm_service(epn),
        etimer,
        eds_port__tick_from_ms(after_ms));
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_NONE, "etimer, agent, event_id, after_ms = (%p, %p, %u, %u)", etimer, agent, event->p__id, after_ms);
    return EDS__ERROR_NONE;
}

eds__error
eds__etimer_send_every(eds__etimer * etimer,
    eds__agent * agent,
    const eds__event * event,
    uint32_t every_ms)
{
    struct eds_object__epn * epn;
	EDS_PORT__CRITICAL_INSTANCE(critical);

    if ((etimer == NULL) || (agent == NULL) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (every_ms < eds_port__tick_duration_ms()) {
        return EDS__ERROR_OUT_OF_RANGE;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    if (eds_epa__is_designated(agent) == false) {
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        return EDS__ERROR_NO_PERMISSION;
    }
    if (eds_etm__is_designated(etimer) == true) {
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        return EDS__ERROR_ALREADY_EXISTS;
    }
    epn = eds_epa__designation(agent);
    eds_etm__designate(etimer, agent); /* Designate that this timer is owned by the agent */
    eds_etm__set_event(etimer, event);
    eds_evt__ref_up(event);
    eds_etm_service__start_periodic(eds_epn__etm_service(epn),
        etimer,
        eds_port__tick_from_ms(every_ms));
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__etimer_cancel(eds__etimer * etimer)
{
    struct eds_object__epn * epn;
	EDS_PORT__CRITICAL_INSTANCE(critical);

    if (etimer == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    if (eds_etm__is_designated(etimer) == false) {
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        return EDS__ERROR_NOT_EXISTS;
    }
    /* To have designated timer but no designated EPA cannot happen. */
    epn = eds_epa__designation(etimer->p__epa);
    eds_etm_service__cancel(eds_epn__etm_service(epn), etimer);
    eds_etm__designate(etimer, NULL); /* Designate that the timer is not owned by any agent */
    eds_evt__null(etimer->p__evt); /* We want to null-ify this event for all receivers */
    eds_evt__ref_down(etimer->p__evt);
    eds_evt__deallocate(etimer->p__evt); /* Dispose of event as well */
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CANCEL, EDS__ERROR_NONE, "etimer = (%p)", etimer);
    return EDS__ERROR_NONE;
}

