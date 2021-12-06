#include "eds_epa.h"
#include "eds_sm.h"
#include "eds_mem.h"
#include "eds_core.h"
#include "eds_port.h"
#include "eds_event.h"
#include "eds_equeue.h"

void
eds_epa__set_epn(struct eds_object__epa *epa, struct eds_object__epn *epn)
{
    epa->p__epn = epn;
}

nk_eds_sm__action
eds_epa__dispatch(struct eds_object__epa *epa)
{

}

nk_eds_error
nk_eds_epa__create(nk_eds_sm__state_fn *initial_state,
    void *sm_workspace,
    const struct nk_eds_epa__attr *attr,
    nk_eds_epa **sm)
{
    static const struct nk_eds_epa__attr default_attr =
        {
            .name = EDS__EPA__DEFAULT_NAME,
            .prio = EDS__EPA__DEFAULT_PRIO,
            .instance = NULL,
            .equeue_size = EDS__SM__DEFAULT_QUEUE_N_OF_ENTRIES,
            .equeue_storage = NULL
        };
    struct eds_object__epa *l_epa;
    struct eds_object__event *equeue_storage;
    struct eds_object__mem *mem;

    if ((initial_state == NULL) || (sm == NULL)) {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    attr = attr != NULL ? attr : &default_attr;
    if ((attr->equeue_size == 0u) || ((attr->instance != NULL) && (attr->equeue_storage == NULL))
        || ((attr->instance == NULL) && (attr->equeue_storage != NULL))) {
        return NK_EDS_ERROR__INVLD_CONFIGURATION;
    }
    if (attr->instance == NULL) {
        struct eds_port__critical critical;
        size_t sm_size_bytes;

        sm_size_bytes = sizeof(struct eds_object__epa)
            + eds_equeue__calculate_storage_size(attr->equeue_size);

        mem = eds_mem__select(sm_size_bytes);
        if (mem == NULL) {
            return NK_EDS_ERROR__NO_RESOURCE;
        }
        eds_port__critical__lock(&critical);
        l_epa = eds_mem__allocate_from(mem, sm_size_bytes);
        eds_port__critical__unlock(&critical);
        if (l_epa == NULL) {
            return NK_EDS_ERROR__NO_MEMORY;
        }
        /* Put the queue storage directly above the SM structure
         */
        equeue_storage = (struct eds_object__event*) (l_epa + 1u);
    } else {
        mem = NULL;
        l_epa = attr->instance;
        equeue_storage = attr->equeue_storage;
    }
    eds_sm__init(&l_epa->p__sm, initial_state, sm_workspace);
    eds_equeue__init(&l_epa->p__equeue, attr->equeue_size, equeue_storage);
    eds_core__escheduler__node_init(&l_epa->p__node, attr->prio);
    l_epa->p__mem = mem;
    l_epa->p__epn = NULL;
    l_epa->p__name = attr->name != NULL ? attr->name : EDS__EPA__DEFAULT_NAME;
    *sm = l_epa;
    return NK_EDS_ERROR__NONE;
}

nk_eds_error
nk_eds_epa__delete(nk_eds_epa *epa)
{
    struct eds_port__critical critical;

    if (epa == NULL) {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    eds_port__critical__lock(&critical);
    /* Clear the queue */
    while (!eds_equeue__is_empty(&epa->p__equeue)) {
        const struct eds_object__event *event;

        event = eds_equeue__pop(&epa->p__equeue);
        eds_event__deallocate(event);
    }
    /* If this SM was already added to EPA */
    if (epa->p__epn != NULL) {
        eds_core__escheduler__block(&epa->p__epn->p__scheduler, &epa->p__node);
    }
    if (epa->p__mem != NULL) {
        eds_mem__deallocate_to(epa->p__mem, epa);
    }
    eds_port__critical__unlock(&critical);

    return NK_EDS_ERROR__NONE;
}

nk_eds_error
nk_eds_epa__send_signal(nk_eds_epa *epa, uint32_t event_id)
{
    struct eds_object__event *event;
    nk_eds_error error;

    error = nk_eds_event__create(event_id, 0u, &event);

    if (error) {
        return error;
    }
    error = nk_eds_epa__send_event(epa, event);

    if (error) {
        nk_eds_event__cancel(event);
    }
    return error;
}

nk_eds_error
nk_eds_epa__send_event(nk_eds_epa *epa, const nk_eds_event *event)
{
    struct eds_port__critical critical;

    if ((epa == NULL) || (event == NULL)) {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    if (epa->p__epn == NULL) {
        return NK_EDS_ERROR__NO_PERMISSION;
    }
    eds_port__critical__lock(&critical);
    eds_event__ref_up(event);
    eds_equeue__push(&epa->p__equeue, event);
    eds_core__escheduler__ready(&epa->p__epn->p__scheduler, &epa->p__node);
    eds_port__critical__unlock(&critical);
    return NK_EDS_ERROR__NONE;
}

