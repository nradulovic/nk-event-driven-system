#include "eds_epa.h"
#include "eds.h"
#include "eds_smp.h"
#include "eds_mem.h"
#include "eds_evt.h"
#include "eds_core.h"
#include "eds_port.h"
#include "eds_equeue.h"

void
eds_epa__set_epn(struct eds_object__epa *agent, struct eds_object__epn *epn)
{
    agent->p__epn = epn;
}

eds__sm_action
eds_epa__dispatch(struct eds_object__epa *agent)
{
    return EDS__SM__ACTION__HANDLED;
}

void
eds_epa__deallocate(struct eds_object__epa *epa)
{

}

eds__error
eds__agent_create(eds__sm_state *sm_initial_state,
    void *sm_workspace,
    const struct eds__agent_attr *attr,
    eds__agent **agent)
{
    static const struct eds__agent_attr default_attr =
        {
            .name = EDS__DEFAULT_EPA_NAME,
            .prio = EDS__DEFAULT_EPA_PRIO,
            .equeue_entries = EDS__DEFAULT_EPA_QUEUE_ENTRIES,
            .static_instance = NULL,
            .static_equeue_storage = NULL
        };
    struct eds_object__epa *epa;
    struct eds_object__evt *equeue_storage;
    struct eds_object__mem *mem;

    if ((sm_initial_state == NULL) || (agent == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    attr = attr != NULL ? attr : &default_attr;
    if ((attr->equeue_entries == 0u) || ((attr->static_instance != NULL) && (attr->static_equeue_storage == NULL))
        || ((attr->static_instance == NULL) && (attr->static_equeue_storage != NULL))) {
        return EDS__ERROR_INVALID_CONFIGURATION;
    }
    if (attr->static_instance == NULL) {
        struct eds_port__critical critical;
        size_t sm_size_bytes;

        sm_size_bytes = sizeof(struct eds_object__epa)
            + eds_equeue__calculate_storage_size(attr->equeue_entries);

        mem = eds_mem__select(sm_size_bytes);
        if (mem == NULL) {
            return EDS__ERROR_NO_RESOURCE;
        }
        eds_port__critical_lock(&critical);
        epa = eds_mem__allocate_from(mem, sm_size_bytes);
        eds_port__critical_unlock(&critical);
        if (epa == NULL) {
            return EDS__ERROR_NO_MEMORY;
        }
        /* Put the queue storage directly above the SM structure
         */
        equeue_storage = (struct eds_object__evt*) (epa + 1u);
    } else {
        mem = NULL;
        epa = attr->static_instance;
        equeue_storage = attr->static_equeue_storage;
    }
    eds_smp__init(&epa->p__sm, sm_initial_state, sm_workspace);
    eds_equeue__init(&epa->p__equeue, attr->equeue_entries, equeue_storage);
    eds_core__etask_init(&epa->p__etask, attr->prio);
    epa->p__mem = mem;
    epa->p__epn = NULL;
    epa->p__name = attr->name != NULL ? attr->name : EDS__DEFAULT_EPA_NAME;
    *agent = epa;
    return EDS__ERROR_NONE;
}

eds__error
eds__agent_delete(eds__agent *agent)
{
    struct eds_port__critical critical;

    if (agent == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    eds_port__critical_lock(&critical);
    /* Clear the queue */
    while (!eds_equeue__is_empty(&agent->p__equeue)) {
        const struct eds_object__evt *event;

        event = eds_equeue__pop(&agent->p__equeue);
        eds_evt__deallocate(event);
    }
    /* If this SM was already added to EPA */
    if (agent->p__epn != NULL) {
        eds_core__escheduler__block(&agent->p__epn->p__scheduler, &agent->p__etask);
    }
    if (agent->p__mem != NULL) {
        eds_mem__deallocate_to(agent->p__mem, agent);
    }
    eds_port__critical_unlock(&critical);

    return EDS__ERROR_NONE;
}

eds__error
eds__agent_signal(eds__agent *agent, uint32_t event_id)
{
    struct eds_object__evt *event;
    eds__error error;

    error = eds__event_create(event_id, 0u, &event);

    if (error) {
        return error;
    }
    error = eds__agent_send(agent, event);

    if (error) {
        eds__event_cancel(event);
    }
    return error;
}

eds__error
eds__agent_send(eds__agent *agent, const eds__event *event)
{
    struct eds_port__critical critical;

    if ((agent == NULL) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (agent->p__epn == NULL) {
        return EDS__ERROR_NO_PERMISSION;
    }
    eds_port__critical_lock(&critical);
    eds_evt__ref_up(event);
    eds_equeue__push_back(&agent->p__equeue, event);
    eds_core__escheduler__ready(&agent->p__epn->p__scheduler, &agent->p__etask);
    eds_port__critical_unlock(&critical);
    return EDS__ERROR_NONE;
}

