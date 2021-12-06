
#include "nk_eds.h"
#include "eds_core.h"
#include "eds_event.h"
#include "eds_port.h"

nk_eds_error
eds_sm__create(eds_sm__state_fn *         initial_state,
               void *                     sm_workspace,
               const struct eds_sm__attr *attr,
               eds_sm **                  sm)
{
    static const struct eds_sm__attr default_attr =
    {
        .name = EDS__SM__DEFAULT_NAME,
        .prio = EDS__SM__DEFAULT_PRIO,
        .instance = NULL,
        .equeue_size = EDS__SM__DEFAULT_QUEUE_N_OF_ENTRIES,
        .equeue_storage = NULL
    };
    struct eds_object__sm * l_sm;
    struct eds_object__event * equeue_storage;
    struct eds_object__mem * mem;

    if ((initial_state == NULL) || (sm == NULL)) {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    attr = attr != NULL ? attr : &default_attr;
    if ((attr->equeue_size == 0u) ||
        ((attr->instance != NULL) && (attr->equeue_storage == NULL)) ||
        ((attr->instance == NULL) && (attr->equeue_storage != NULL))) {
        return NK_EDS_ERROR__INVLD_CONFIGURATION;
    }
    if (attr->instance == NULL) {
        size_t sm_size_bytes;

        sm_size_bytes = sizeof(struct eds_object__sm) +
                eds_core__equeue__calculate_storage_size(attr->equeue_size);
        mem = eds_core__mem__select(sm_size_bytes);
        if (mem == NULL) {
            return NK_EDS_ERROR__NO_RESOURCE;
        }
        l_sm = eds_core__mem__allocate_from(mem, sm_size_bytes);
        if (l_sm == NULL) {
            return NK_EDS_ERROR__NO_MEMORY;
        }
        /* Put the queue storage directly above the SM structure
         */
        equeue_storage = (struct eds_object__event *)(l_sm + 1u);
    } else {
        mem = NULL;
        l_sm = attr->instance;
        equeue_storage = attr->equeue_storage;
    }
    eds_core__sm_executor__init(&l_sm->p__sm_executor, initial_state, sm_workspace);
    eds_core__equeue__init(&l_sm->p__equeue, attr->equeue_size, equeue_storage);
    eds_core__escheduler__node_init(&l_sm->p__node, attr->prio);
    l_sm->p__epa = NULL;
    l_sm->p__mem = mem;
    l_sm->p__name = attr->name != NULL ? attr->name : EDS__SM__DEFAULT_NAME;
    *sm = l_sm;
    return NK_EDS_ERROR__NONE;
}

nk_eds_error
eds_sm__delete(eds_sm *sm)
{
    struct eds_port__critical critical;

    if (sm == NULL)
    {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    eds_port__critical__lock(&critical);
    /* Clear the queue */
    while (!eds_core__equeue__is_empty(&sm->p__equeue))
    {
        const struct eds_object__event * event;

        event = eds_core__equeue__pop(&sm->p__equeue);
        eds_event__deallocate(event);
    }
    /* If this SM was already added to EPA */
    if (sm->p__epa != NULL)
    {
        eds_core__escheduler__block(&sm->p__epa->scheduler, &sm->p__node);
    }
    eds_port__critical__unlock(&critical);

    return NK_EDS_ERROR__NONE;
}

nk_eds_error
eds_sm__send_signal(eds_sm * sm,
                    uint32_t event_id)
{
    struct eds_object__event * event;
    nk_eds_error error;

    error = nk_eds_event__create(event_id, 0u, &event);

    if (error) {
        return error;
    }
    error = nk_eds_sm__send_event(sm, event);

    if (error) {
        nk_eds_event__cancel(event);
    }
    return error;
}

nk_eds_error
nk_eds_sm__send_event(eds_sm *sm, const nk_eds_event *event)
{
    struct eds_port__critical critical;

    if ((sm == NULL) || (event == NULL)) {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    if (sm->p__epa == NULL) {
        return NK_EDS_ERROR__NO_PERMISSION;
    }
    eds_port__critical__lock(&critical);
    eds_event__ref_up(event);
    eds_port__critical__unlock(&critical);
}
