/*
 * eds.c
 *
 *  Created on: Dec 13, 2021
 *      Author: nenad
 */
#include "eds.h"
#include "eds_object.h"
#include "eds_port.h"
#include "sys/eds_epa.h"
#include "sys/eds_epn.h"
#include "sys/eds_equeue.h"
#include "sys/eds_etm.h"
#include "sys/eds_evt.h"
#include "sys/eds_mem.h"
#include "sys/eds_smp.h"
#include "sys/eds_tmr.h"

#include <assert.h>

static struct eds_object__list eds__epn_list = EDS_CORE__LIST_INITIALIZER(&eds__epn_list);

eds__error
eds__mem_add_allocator(eds__mem_alloc_fn *alloc,
    eds__mem_dealloc_fn *dealloc,
    void *context,
    size_t max_size)
{
    if ((alloc == NULL) || (dealloc == NULL) || (max_size < EDS__DEFAULT_MEM_MIN_BYTES)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_core__vector_is_full(&mem__instances) == true) {
        return EDS__ERROR_NO_RESOURCE;
    }
    return eds_mem__add_allocator(alloc, dealloc, context, max_size);
}

eds__error
eds__event_create(uint32_t event_id, size_t event_data_size, eds__event **event)
{
    struct eds_object__evt *l_event;
    eds_core__error core_error;

    if ((event_id == 0) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    core_error = eds_evt__allocate(event_id, event_data_size, &l_event);
    switch (core_error) {
    case EDS_CORE__ERROR__NO_RESOURCE:
        return EDS__ERROR_NO_RESOURCE;
    case EDS_CORE__ERROR__NO_MEMORY:
        return EDS__ERROR_NO_MEMORY;
    default:
        break;
    }
    *event = l_event;
    return EDS__ERROR_NONE;
}

eds__error
eds__event_cancel(eds__event *event)
{
    struct eds_port__critical critical;

    if (event == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (event->p__mem == NULL) {
        return EDS__ERROR_NO_PERMISSION;
    }
    eds_port__critical_lock(&critical);
    if (eds_evt__is_in_use(event)) {
        event->p__id = EDS__EVENT__NULL;
    } else {
        eds_mem__deallocate_to(event->p__mem, event);
    }
    eds_port__critical_unlock(&critical);

    return EDS__ERROR_NONE;
}

eds__error
eds__event_init(eds__event *event, uint32_t event_id, size_t event_data_size)
{
    if ((event_id == 0) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    eds_evt__init(event, event_id, event_data_size, NULL);

    return EDS__ERROR_NONE;
}

uint32_t
eds__event_id(const eds__event *event)
{
    assert(event != NULL);
    return event->p__id;
}

void*
eds__event_data(eds__event *event)
{
    assert(event != NULL);
    if (event->p__size != 0u) {
        return event + 1u;
    } else {
        return NULL;
    }
}

size_t
eds__event_size(const eds__event *event)
{
    assert(event != NULL);
    return event->p__size;
}

eds__error
eds__agent_create(eds__sm_state *sm_initial_state,
    void *sm_workspace,
    const struct eds__agent_attr *attr,
    eds__agent **agent)
{
    static const struct eds__agent_attr default_attr =
        {
        .name = EDS__DEFAULT_EPA_NAME, .prio = EDS__DEFAULT_EPA_PRIO, .equeue_entries =
        EDS__DEFAULT_EPA_QUEUE_ENTRIES, .static_instance = NULL, .static_equeue_storage = NULL
        };
    eds__error error;

    if ((sm_initial_state == NULL) || (agent == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    attr = attr != NULL ? attr : &default_attr;
    if ((attr->equeue_entries == 0u)
        || ((attr->static_instance != NULL) && (attr->static_equeue_storage == NULL))
        || ((attr->static_instance == NULL) && (attr->static_equeue_storage != NULL))) {
        return EDS__ERROR_INVALID_CONFIGURATION;
    }
    error = eds_epa__create(sm_initial_state, sm_workspace, attr, agent);
    return error;
}

eds__error
eds__agent_delete(eds__agent *agent)
{
    struct eds_port__critical critical;

    if (agent == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_epa__is_designated(agent) == false) {
        return EDS__ERROR_NO_PERMISSION;
    }
    eds_port__critical_lock(&critical);
    /* Clear the queue */
    while (!eds_equeue__is_empty(&agent->p__equeue)) {
        const struct eds_object__evt *event;

        event = eds_equeue__pop(&agent->p__equeue);
        eds_evt__deallocate(event);
    }
    if (agent->p__mem != NULL) {
        eds_mem__deallocate_to(agent->p__mem, agent);
    }
    eds_port__critical_unlock(&critical);

    return EDS__ERROR_NONE;
}

eds__error
eds__agent_send(eds__agent *agent, const eds__event *event)
{
    struct eds_port__critical critical;
    eds__error error;
    eds_core__error core_error;

    if ((agent == NULL) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_epa__is_designated(agent) == false) {
        return EDS__ERROR_NO_PERMISSION;
    }
    eds_port__critical_lock(&critical);
    core_error = eds_epa__send(agent, event);
    eds_port__critical_unlock(&critical);
    switch (core_error) {
    case EDS_CORE__ERROR__NO_SPACE:
        error = EDS__ERROR_NO_SPACE;
        break;
    default:
        error = EDS__ERROR_NONE;
        break;
    }
    return error;
}

eds__error
eds__agent_send_after(eds__agent *agent, const eds__event *event, uint32_t after_ms)
{
    if ((agent == NULL) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    return EDS__ERROR_NONE;
}

eds__error
eds__agent_send_every(eds__agent *agent, const eds__event *event, uint32_t every_ms)
{
    return EDS__ERROR_NONE;
}

eds__agent*
eds__agent_from_sm(eds__sm *sm)
{
    return EDS_CORE__CONTAINER_OF(sm, struct eds_object__epa, p__smp);
}

eds__network*
eds__agent_network(const eds__agent *agent)
{
    return eds_epa__designation(agent);
}

eds__error
eds__etimer_create(const struct eds__etimer_attr *attr, eds__etimer **etimer)
{
    static const struct eds__etimer_attr default_attr;

    if (etimer == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (attr == NULL) {
        attr = &default_attr;
    }
    return eds_etm__create(attr, etimer);
}

eds__error
eds__etimer_delete(eds__etimer *etimer)
{
    struct eds_port__critical critical;

    if (etimer == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    eds_port__critical_lock(&critical);
    if (eds_etm__is_designated(etimer) == false) {
        eds_port__critical_unlock(&critical);
        return EDS__ERROR_NOT_EXISTS;
    }
    if (etimer->p__mem != NULL) {
        eds_mem__deallocate_to(etimer->p__mem, etimer);
    }
    eds_port__critical_unlock(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__etimer_send_after(eds__etimer *etimer,
    eds__agent *agent,
    const eds__event *event,
    uint32_t after_ms)
{
    struct eds_port__critical critical;
    struct eds_object__epn *epn;

    if ((etimer == NULL) || (agent == NULL) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (after_ms < eds_port__tick_duration_ms()) {
        return eds__agent_send(agent, event);
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
    eds_etm_service__start_once(eds_epn__etm_service(epn), etimer, eds_port__tick_from_ms(after_ms));
    eds_port__critical_unlock(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__etimer_send_every(eds__etimer *etimer,
    eds__agent *agent,
    const eds__event *event,
    uint32_t every_ms)
{
    struct eds_port__critical critical;
    struct eds_object__epn *epn;

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
    eds_etm_service__start_periodic(eds_epn__etm_service(epn), etimer, eds_port__tick_from_ms(every_ms));
    eds_port__critical_unlock(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__etimer_cancel(eds__etimer *etimer)
{
    struct eds_port__critical critical;
    struct eds_object__epn *epn;

    if (etimer == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    eds_port__critical_lock(&critical);
    if (eds_etm__is_designated(etimer) == false) {
        eds_port__critical_unlock(&critical);
        return EDS__ERROR_NOT_EXISTS;
    }
    if (eds_epa__is_designated(etimer->p__epa) == false) {
        eds_port__critical_unlock(&critical);
        return EDS__ERROR_NO_PERMISSION;
    }
    epn = eds_epa__designation(etimer->p__epa);
    eds_etm_service__cancel(eds_epn__etm_service(epn), etimer);
    eds_etm__designate(etimer, NULL); /* Designate that the timer is not owned by any agent */
    eds_port__critical_unlock(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__epn_create(const struct eds__epn_attr *attr, eds__network **network)
{
    static const struct eds__epn_attr default_attr =
        {
        .name = EDS__DEFAULT_EPN_NAME
        };
    struct eds_object__epn *epn;
    struct eds_object__mem *mem;
    struct eds_port__critical critical;

    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (attr == NULL) {
        attr = &default_attr;
    }
    if (attr->instance == NULL) {
        mem = eds_mem__select(sizeof(*epn));
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
    epn->p__name = attr->name != NULL ? attr->name : EDS__DEFAULT_EPN_NAME;
    *network = epn;
    eds_port__critical_lock(&critical);
    eds_core__list_add_after(&epn->p__list, &eds__epn_list);
    eds_port__critical_unlock(&critical);
    return EDS__ERROR_NONE;
}

eds__error
eds__epn_delete(eds__network *epn)
{
    struct eds_port__critical critical;

    if (epn == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (epn->p__mem == NULL) {
        return EDS__ERROR_NO_PERMISSION;
    }
    eds_port__critical_lock(&critical);
    eds_core__list_remove(&epn->p__list);
    // TODO
    eds_port__critical_unlock(&critical);

    return EDS__ERROR_NONE;
}

eds__error
eds__epn_add_epa(eds__network *network, eds__agent *agent)
{
    if ((network == NULL) || (agent == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_epa__is_designated(agent) == false) {
        return EDS__ERROR_ALREADY_EXISTS;
    }
    eds_epa__designate(agent, network);
    return EDS__ERROR_NONE;
}

eds__error
eds__epn_remove_epa(eds__network *network, eds__agent *agent)
{
    if ((network == NULL) || (agent == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (eds_epa__is_designated(agent) == false) {
        return EDS__ERROR_NOT_EXISTS;
    }
    eds_epa__designate(agent, NULL);
    return EDS__ERROR_NONE;
}

eds__error
eds__epn_start(eds__network *network)
{
    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    while (network->p__should_run) {
        struct eds_object__tasker_node *current;
        struct eds_port__critical critical;
        eds_core__error core_error;

        eds_port__critical_lock(&critical);
        while ((current = eds_core__tasker_highest(&network->p__tasker)) != NULL) {
            struct eds_object__epa *current_epa;

            current_epa = EDS_CORE__CONTAINER_OF(current, struct eds_object__epa, p__task);
            core_error = eds_epa__dispatch(current_epa, &critical);
            if (core_error != EDS_CORE__ERROR_NONE) {
                break;
            }
        }
        eds_port__critical_unlock(&critical);
        switch (core_error) {
        case EDS_CORE__ERROR__BAD_STATE:
            return EDS__ERROR_MALFORMED_SM;
        default:
            break;
        }
        eds_epn__sleep_wait(network);
    }
    return EDS__ERROR_NONE;
}

eds__error
eds__epn_stop(eds__network *network)
{
    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    // TODO
    network->p__should_run = false;
    return EDS__ERROR_NONE;
}

eds__error
eds__epn_process_tick(void)
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
