#include "sys/eds_epn.h"
#include "sys/eds_epa.h"
#include "sys/eds_mem.h"
#include "sys/eds_state.h"
#include "sys/eds_core.h"
#include "sys/eds_etm.h"
#include "eds_trace.h"
#include "eds_port.h"

static eds__error
eds_epn__allocate(const struct eds__network_attr * attr, struct eds_object__epn ** epn, struct eds_object__mem ** mem)
{
    struct eds_object__epn * l_epn;
    struct eds_object__mem * l_mem;

    l_mem = eds_mem__find(&eds_state__mem_instances, sizeof(*l_epn));
    if (l_mem == NULL) {
        return EDS__ERROR_NO_RESOURCE;
    }
    eds_port__critical_lock();
    l_epn = eds_mem__allocate_from(l_mem, sizeof(*l_epn));
    eds_port__critical_unlock();
    if (l_epn == NULL) {
        return EDS__ERROR_NO_MEMORY;
    }
    *mem = l_mem;
    *epn = l_epn;
    return EDS__ERROR_NONE;
}

static void
eds_epn__initialize(struct eds_object__epn * epn, struct eds_object__mem * mem, const struct eds__network_attr * attr)
{
    eds_core__list_init(&epn->p__list);
    eds_core__tasker_init(&epn->p__tasker);
#if (EDS_PORT__USE_GLOBAL_SLEEP == 0)
    eds_port__sleep_init(&epn->p__sleep);
#endif
    epn->p__should_run = true;
    epn->p__mem = mem;
    epn->p__attr = attr;
}

static eds__error
eds_epn__create(const struct eds__network_attr * attr, struct eds_object__epn ** epn)
{
    static const struct eds__network_attr default_attr =
    {
        .name = EDS__DEFAULT_NETWORK_NAME
    };
    struct eds_object__epn * l_epn;
    struct eds_object__mem * mem;

    if (attr == NULL) {
        attr = &default_attr;
    }
    if (attr->instance != NULL) {
        l_epn = attr->instance;
        mem = NULL;
    } else {
        eds__error error;
        
        error = eds_epn__allocate(attr, &l_epn, &mem);
        if (error != EDS__ERROR_NONE) {
            return error;
        }
    }
    eds_epn__initialize(l_epn, mem, attr);
    *epn = l_epn;
    eds_state__has_started = true;
    return EDS__ERROR_NONE;
}

static eds__error
eds_epn__delete(struct eds_object__epn * epn)
{
    if (epn->p__mem == NULL) {
        return EDS__ERROR_NO_PERMISSION;
    }
    eds_port__critical_lock();
    // TODO: Delete all associated EPAs
    eds_mem__deallocate_to(epn->p__mem, epn);
    eds_port__critical_unlock();
    
    return EDS__ERROR_NONE;
}

static eds__error
eds_epn__add_agent(struct eds_object__epn * epn, struct eds_object__epa * epa)
{
    eds__error error;
    
    eds_port__critical_lock();
    if (eds_epa__is_designated(epa) == true) {
        eds_port__critical_unlock();
        return EDS__ERROR_ALREADY_EXISTS;
    }
    eds_epa__designate(epa, epn);
    error = eds_epa__send_initial_event(epa);
    // TODO: Add EPA to EPN list
    eds_port__critical_unlock();
    return error;
}

static eds__error
eds_epn__remove_agent(struct eds_object__epn * epn, struct eds_object__epa * epa)
{
	eds_port__critical_lock();
    if (eds_epa__is_designated(epa) == false) {
    	eds_port__critical_unlock();
        return EDS__ERROR_NOT_EXISTS;
    }
    eds_etm_service__delete_all(epa);
    eds_epa__flush_events(epa);
    eds_epa__designate(epa, NULL);
    eds_port__critical_unlock();
    return EDS__ERROR_NONE;
}

static void
eds_epn__pre_loop(struct eds_object__epn * epn)
{
	eds_port__critical_lock();
    if (eds_state__is_eds_initialized == false) {
        eds_state__is_eds_initialized = true;
        eds_port__critical_unlock();
        eds_port__init();
        eds_etm_service__init();
#if (EDS_PORT__USE_GLOBAL_SLEEP == 1)
        eds_port__sleep_init(&eds_port__global_sleep);
#endif
        eds_port__critical_lock();
    }
    eds_core__list_add_after(&epn->p__list, &eds_state__epn_instances);
    eds_port__critical_unlock();
}

static eds__error
eds_epn__loop(struct eds_object__epn * epn)
{
    while (epn->p__should_run) {
    	eds_port__critical_lock();
        struct eds_object__tasker_node * tasker_node;

        while ((tasker_node = eds_core__tasker_highest(&epn->p__tasker)) != NULL) {
            eds__error error;
            struct eds_object__epa * epa;

            epa = eds_epa__from_tasker_node(tasker_node);
            error = eds_epa__dispatch(epa);
            if (error != EDS__ERROR_NONE) {
            	eds_port__critical_unlock();
                return error;
            }
        }
        eds_port__critical_unlock();
#if (EDS_CONFIG__NETWORK__ENABLE_CALLBACKS == 1)
        if (epn->p__attr->cb_to_idle != NULL) {
            epn->p__attr->cb_to_idle(epn, epn->p__attr->cb_arg);
        }
#endif
#if (EDS_PORT__USE_GLOBAL_SLEEP == 0)
        eds_port__sleep_wait(&epn->p__sleep);
#else
        eds_port__sleep_wait(&eds_port__global_sleep);
#endif
#if (EDS_CONFIG__NETWORK__ENABLE_CALLBACKS == 1)
        if (epn->p__attr->cb_to_run != NULL) {
            epn->p__attr->cb_to_run(epn, epn->p__attr->cb_arg);
        }
#endif
    }
    return EDS__ERROR_NONE;
}

static void
eds_epn__post_loop(struct eds_object__epn * epn)
{
    eds_port__critical_lock();
    eds_core__list_remove(&epn->p__list);
    eds_port__critical_unlock();
}

eds__error
eds__network_create(const struct eds__network_attr * attr, eds__network ** network)
{
    eds__error error;
    
    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    error = eds_epn__create(attr, network);
    // TODO: trace error
    return error;
}

eds__error
eds__network_delete(eds__network * network)
{
    eds__error error;
    
    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    error = eds_epn__delete(network);
    // TODO: trace error
    return error;
}

eds__error
eds__network_add_agent(eds__network * network, eds__agent * agent)
{
    eds__error error;

    EDS_TRACE__INFO(EDS_TRACE__SOURCE_NETWORK_ADD_AGENT, "%p <- %p", network, agent);

    if ((network == NULL) || (agent == NULL)) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_NETWORK_ADD_AGENT, EDS__ERROR_INVALID_ARGUMENT, "(n=%p, a=%p)", network, agent);
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    error = eds_epn__add_agent(network, agent);
    EDS_TRACE__EXIT(EDS_TRACE__SOURCE_NETWORK_ADD_AGENT, error, "a=%p <= e=%p", agent, initial_event);

    return error;
}

eds__error
eds__network_remove_agent(eds__network * network, eds__agent * agent)
{
    eds__error error;

    if ((network == NULL) || (agent == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    error = eds_epn__remove_agent(network, agent);
    // TODO: trace_error
    return error;
}

eds__error
eds__network_start(eds__network * network)
{
    eds__error error;

    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    eds_epn__pre_loop(network);
    error = eds_epn__loop(network);
    eds_epn__post_loop(network);
    // TODO: trace_error
    return error;
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

