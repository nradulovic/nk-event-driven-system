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
eds__network_create(const struct eds__network_attr * attr, eds__network ** network)
{
    static const struct eds__network_attr default_attr =
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
        EDS_PORT__CRITICAL_INSTANCE(critical);

        mem = eds_mem__find(&eds_state__mem_instances, sizeof(*epn));
        if (mem == NULL) {
            return EDS__ERROR_NO_RESOURCE;
        }
        EDS_PORT__CRITICAL_LOCK(&critical);
        epn = eds_mem__allocate_from(mem, sizeof(*epn));
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        if (epn == NULL) {
            return EDS__ERROR_NO_MEMORY;
        }
    } else {
        mem = NULL;
        epn = attr->instance;
    }
    epn->p__should_run = true;
    eds_core__tasker_init(&epn->p__tasker);
    eds_etm_service__init(eds_epn__etm_service(epn));
#if (EDS_PORT__USE_LOCAL_SLEEP == 1)
    eds_port__sleep_local_init(&epn->p__sleep);
#else
    eds_port__sleep_global_init(void);
#endif
    epn->p__mem = mem;
    epn->p__attr = attr;
    *network = epn;

    return EDS__ERROR_NONE;
}

eds__error
eds__network_delete(eds__network * network)
{
    EDS_PORT__CRITICAL_INSTANCE(critical);

    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (network->p__mem == NULL) {
        return EDS__ERROR_NO_PERMISSION;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    eds_mem__deallocate_to(network->p__mem, network);
    EDS_PORT__CRITICAL_UNLOCK(&critical);

    return EDS__ERROR_NONE;
}

eds__error
eds__network_add_agent(eds__network * network, eds__agent * agent)
{
    EDS_PORT__CRITICAL_INSTANCE(critical);
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
    EDS_PORT__CRITICAL_LOCK(&critical);
    error = eds_epa__send(agent, initial_event);
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_NETWORK_ADD_AGENT, error, "a=%p <= e=%p", agent, initial_event);

    return error;
}

eds__error
eds__network_remove_agent(eds__network * network, eds__agent * agent)
{
    EDS_PORT__CRITICAL_INSTANCE(critical);

    if ((network == NULL) || (agent == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_epa__is_designated(agent) == false) {
        return EDS__ERROR_NOT_EXISTS;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    eds_etm_service__delete_all(&network->etm_sentinel, agent);
    eds_epa__terminate(agent);
    eds_epa__designate(agent, NULL);
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__network_start(eds__network * network)
{
    static bool is_port_initialized;
    EDS_PORT__CRITICAL_INSTANCE(critical);

    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (is_port_initialized == false) {
        is_port_initialized = true;
        eds_port__init();
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    eds_core__list_add_after(&network->p__list, &eds__epn_list);
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    while (network->p__should_run) {
        struct eds_object__tasker_node * current;

        eds__error error = EDS__ERROR_NONE;

        EDS_PORT__CRITICAL_LOCK(&critical);
        while ((current = eds_core__tasker_highest(&network->p__tasker)) != NULL) {
            struct eds_object__epa * current_epa;

            current_epa = EDS_CORE__CONTAINER_OF(current, struct eds_object__epa, p__task);
#if (EDS_PORT__USE_LOCAL_CRITICAL == 1)
            error = eds_epa__dispatch(current_epa, &critical);
#else
        error = eds_epa__dispatch(current_epa);
#endif
            if (error != EDS__ERROR_NONE) {
                break;
            }
        }
        EDS_PORT__CRITICAL_UNLOCK(&critical);
        if (error != EDS__ERROR_NONE) {
            return error;
        }
#if (EDS_CONFIG__NETWORK__ENABLE_CALLBACKS == 1)
        if (network->p__attr->cb_to_idle != NULL) {
            network->p__attr->cb_to_idle(network, network->p__attr->cb_arg);
        }
#endif
        eds_epn__sleep_wait(network);
#if (EDS_CONFIG__NETWORK__ENABLE_CALLBACKS == 1)
        if (network->p__attr->cb_to_run != NULL) {
            network->p__attr->cb_to_run(network, network->p__attr->cb_arg);
        }
#endif
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    eds_core__list_remove(&network->p__list);
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__network_stop(eds__network * network)
{
    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    network->p__should_run = false;
    return EDS__ERROR_NONE;
}

eds__error
eds__network_name(eds__network * network, const char ** name)
{
    if ((network == NULL) || (name == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    *name = network->p__attr->name != NULL ? network->p__attr->name : EDS__DEFAULT_NETWORK_NAME;
    return EDS__ERROR_NONE;
}

eds__error
eds__tick_process_all(void)
{
    struct eds_object__list * current;
    EDS_PORT__CRITICAL_INSTANCE(critical);

    EDS_PORT__CRITICAL_LOCK(&critical);
    for (EDS_CORE__LIST_EACH(current, &eds__epn_list)) {
        struct eds_object__epn * epn;

        epn = EDS_CORE__CONTAINER_OF(current, struct eds_object__epn, p__list);
        eds_etm_service__tick(&epn->etm_sentinel);
    }
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    return EDS__ERROR_NONE;
}
