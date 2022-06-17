/*
 * eds.c
 *
 *  Created on: Dec 13, 2021
 *      Author: nenad
 */
#include "eds.h"
#include "eds_object.h"
#include "eds_port.h"
#include "eds_trace.h"
#include "sys/eds_epa.h"
#include "sys/eds_epn.h"
#include "sys/eds_equeue.h"
#include "sys/eds_etm.h"
#include "sys/eds_evt.h"
#include "sys/eds_mem.h"
#include "sys/eds_smp.h"
#include "sys/eds_tmr.h"
#include "sys/eds_state.h"

#include <assert.h>

static struct eds_object__list eds__epn_list = EDS_CORE__LIST_INITIALIZER(&eds__epn_list)
;

const char*
eds__error_to_str(uint32_t error)
{
    static const char * error_texts[] =
    {
        [EDS__ERROR_NONE] = "none",
        [EDS__ERROR_INVALID_ARGUMENT] = "invalid argument",
        [EDS__ERROR_NO_MEMORY] = "no memory",
        [EDS__ERROR_NO_RESOURCE] = "no resource",
        [EDS__ERROR_NO_PERMISSION] = "no permission",
        [EDS__ERROR_ALREADY_EXISTS] = "already exists",
        [EDS__ERROR_INVALID_CONFIGURATION] = "invalid configuration",
        [EDS__ERROR_NOT_EXISTS] = "not exists",
        [EDS__ERROR_MALFORMED_SM] = "malformed SM",
        [EDS__ERROR_NO_SPACE] = "no space",
        [EDS__ERROR_OUT_OF_RANGE] = "out of range",
    };
    const char * error_text = NULL;

    if (error < (sizeof(error_texts) / sizeof(error_texts[0]))) {
        error_text = error_texts[error];
    }

    if (error_text != NULL) {
        return error_text;
    } else {
        return "unknown";
    }
}

eds__sm_action
eds__sm_event_handled(eds__sm * sm)
{
    return eds_smp__action_handled(sm);
}

eds__sm_action
eds__sm_event_ignored(eds__sm * sm)
{
    return eds_smp__action_ignored(sm);
}

eds__sm_action
eds__sm_transit_to(eds__sm * sm, eds__sm_state * new_state)
{
    return eds_smp__action_transit(sm, new_state);
}

eds__sm_action
eds__sm_super_state(eds__sm * sm, eds__sm_state * super_state)
{
    return eds_smp__action_super(sm, super_state);
}

eds__sm_action
eds__sm_top_state(eds__sm * sm, void * workspace, const eds__event * event)
{
    (void)sm;
    (void)workspace;
    (void)event;

    EDS_TRACE__INFO(EDS_TRACE__SOURCE_EDS_SMP, "(%p) ignored event_id %u", sm, event->p__id);

    return EDS__SM__ACTION__IGNORED;
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
    struct eds_port__critical critical;
    struct eds_object__epn * epn;

    if ((etimer == NULL) || (agent == NULL) || (event == NULL)) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_INVALID_ARGUMENT, "etimer, agent, event = (%p, %p, %p)", etimer, agent, event);
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (after_ms < eds_port__tick_duration_ms()) {
        return eds__agent_send(agent, event);
    }
    eds_port__critical_lock(&critical);
    if (eds_epa__is_designated(agent) == false) {
        eds_port__critical_unlock(&critical);
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_NO_PERMISSION, "etimer, agent, event = (%p, %p, %p)", etimer, agent, event);
        return EDS__ERROR_NO_PERMISSION;
    }
    if (eds_etm__is_designated(etimer) == true) {
        eds_port__critical_unlock(&critical);
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
    eds_port__critical_unlock(&critical);
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_SEND_AFTER, EDS__ERROR_NONE, "etimer, agent, event_id, after_ms = (%p, %p, %u, %u)", etimer, agent, event->p__id, after_ms);
    return EDS__ERROR_NONE;
}

eds__error
eds__etimer_send_every(eds__etimer * etimer,
    eds__agent * agent,
    const eds__event * event,
    uint32_t every_ms)
{
    struct eds_port__critical critical;
    struct eds_object__epn * epn;

    if ((etimer == NULL) || (agent == NULL) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (every_ms < eds_port__tick_duration_ms()) {
        return EDS__ERROR_OUT_OF_RANGE;
    }
    eds_port__critical_lock(&critical);
    if (eds_epa__is_designated(agent) == false) {
        eds_port__critical_unlock(&critical);
        return EDS__ERROR_NO_PERMISSION;
    }
    if (eds_etm__is_designated(etimer) == true) {
        eds_port__critical_unlock(&critical);
        return EDS__ERROR_ALREADY_EXISTS;
    }
    epn = eds_epa__designation(agent);
    eds_etm__designate(etimer, agent); /* Designate that this timer is owned by the agent */
    eds_etm__set_event(etimer, event);
    eds_evt__ref_up(event);
    eds_etm_service__start_periodic(eds_epn__etm_service(epn),
        etimer,
        eds_port__tick_from_ms(every_ms));
    eds_port__critical_unlock(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__etimer_cancel(eds__etimer * etimer)
{
    struct eds_port__critical critical;
    struct eds_object__epn * epn;

    if (etimer == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    eds_port__critical_lock(&critical);
    if (eds_etm__is_designated(etimer) == false) {
        eds_port__critical_unlock(&critical);
        return EDS__ERROR_NOT_EXISTS;
    }
    /* To have designated timer but no designated EPA cannot happen. */
    epn = eds_epa__designation(etimer->p__epa);
    eds_etm_service__cancel(eds_epn__etm_service(epn), etimer);
    eds_etm__designate(etimer, NULL); /* Designate that the timer is not owned by any agent */
    eds_evt__null(etimer->p__evt); /* We want to null-ify this event for all receivers */
    eds_evt__ref_down(etimer->p__evt);
    eds_evt__deallocate(etimer->p__evt); /* Dispose of event as well */
    eds_port__critical_unlock(&critical);
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_ETIMER_CANCEL, EDS__ERROR_NONE, "etimer = (%p)", etimer);
    return EDS__ERROR_NONE;
}

eds__error
eds__network_create(const struct eds__epn_attr * attr, eds__network ** network)
{
    static const struct eds__epn_attr default_attr =
        {
            .name = EDS__DEFAULT_NETWORK_NAME
        };
    struct eds_object__epn * epn;
    struct eds_object__mem * mem;

    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (attr == NULL) {
        attr = &default_attr;
    }
    if (attr->instance == NULL) {
        struct eds_port__critical critical;

        mem = eds_mem__find(&eds_state__mem_instances, sizeof(*epn));
        if (mem == NULL) {
            return EDS__ERROR_NO_RESOURCE;
        }
        eds_port__critical_lock(&critical);
        epn = eds_mem__allocate_from(mem, sizeof(*epn));
        eds_port__critical_unlock(&critical);
        if (epn == NULL) {
            return EDS__ERROR_NO_MEMORY;
        }
    } else {
        mem = NULL;
        epn = attr->instance;
    }
    epn->p__should_run = true;
    eds_core__tasker_init(&epn->p__tasker);
    eds_etm_service__init(&epn->p__etm);
    eds_port__sleep_init(&epn->p__sleep);
    epn->p__mem = mem;
#if (EDS_CONFIG__NETWORK__ENABLE_NAME != 0)
    epn->p__name = attr->name != NULL ? attr->name : EDS__DEFAULT_NETWORK_NAME;
#endif
    *network = epn;

    return EDS__ERROR_NONE;
}

eds__error
eds__epn_delete(eds__network * epn)
{
    if (epn == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (epn->p__mem == NULL) {
        return EDS__ERROR_NO_PERMISSION;
    }

    return EDS__ERROR_NONE;
}

eds__error
eds__network_add_agent(eds__network * network, eds__agent * agent)
{
    struct eds_port__critical critical;
    const struct eds_object__evt * const initial_event = &g__smp_events[EDS__SM_EVENT__INIT];
    eds__error error;

    EDS_TRACE__INFO(EDS_TRACE__SOURCE_NETWORK_ADD_AGENT, "%p <- %p", network, agent);

    if ((network == NULL) || (agent == NULL)) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_NETWORK_ADD_AGENT, EDS__ERROR_INVALID_ARGUMENT, "(n=%p, a=%p)", network, agent);
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_epa__is_designated(agent) == true) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_NETWORK_ADD_AGENT, EDS__ERROR_ALREADY_EXISTS, "a=%p -> n=%p", agent, eds_epa__designation(agent));
        return EDS__ERROR_ALREADY_EXISTS;
    }
    eds_epa__designate(agent, network);
    eds_port__critical_lock(&critical);
    error = eds_epa__send(agent, initial_event);
    eds_port__critical_unlock(&critical);
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_NETWORK_ADD_AGENT, error, "a=%p <= e=%p", agent, initial_event);

    return error;
}

eds__error
eds__network_remove_agent(eds__network * network, eds__agent * agent)
{
    struct eds_port__critical critical;

    if ((network == NULL) || (agent == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_epa__is_designated(agent) == false) {
        return EDS__ERROR_NOT_EXISTS;
    }
    eds_port__critical_lock(&critical);
    eds_etm_service__delete_all(&network->p__etm, agent);
    eds_epa__terminate(agent);
    eds_epa__designate(agent, NULL);
    eds_port__critical_unlock(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__network_start(eds__network * network)
{
    static bool is_port_initialized;
    struct eds_port__critical critical;

    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (is_port_initialized == false) {
        is_port_initialized = true;
        eds_port__init();
    }
    eds_port__critical_lock(&critical);
    eds_core__list_add_after(&network->p__list, &eds__epn_list);
    eds_port__critical_unlock(&critical);
    while (network->p__should_run) {
        struct eds_object__tasker_node * current;

        eds__error error = EDS__ERROR_NONE;

        eds_port__critical_lock(&critical);
        while ((current = eds_core__tasker_highest(&network->p__tasker)) != NULL) {
            struct eds_object__epa * current_epa;

            current_epa = EDS_CORE__CONTAINER_OF(current, struct eds_object__epa, p__task);
            error = eds_epa__dispatch(current_epa, &critical);
            if (error != EDS__ERROR_NONE) {
                break;
            }
        }
        eds_port__critical_unlock(&critical);
        if (error != EDS__ERROR_NONE) {
            return error;
        }
        eds_epn__sleep_wait(network);
    }
    eds_port__critical_lock(&critical);
    eds_core__list_remove(&network->p__list);
    eds_port__critical_unlock(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__epn_stop(eds__network * network)
{
    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    // TODO
    network->p__should_run = false;
    return EDS__ERROR_NONE;
}

eds__error
eds__tick_process_all(void)
{
    struct eds_port__critical critical;
    struct eds_object__list * current;

    eds_port__critical_lock(&critical);
    for (EDS_CORE__LIST_EACH(current, &eds__epn_list)) {
        struct eds_object__epn * epn;

        epn = EDS_CORE__CONTAINER_OF(current, struct eds_object__epn, p__list);
        eds_etm_service__tick(&epn->p__etm);
    }
    eds_port__critical_unlock(&critical);
    return EDS__ERROR_NONE;
}
