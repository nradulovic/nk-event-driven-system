
#include "nk_eds.h"
#include "eds_core.h"

eds_error
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
        return EDS_ERROR__INVLD_ARGUMENT;
    }
    attr = attr != NULL ? attr : &default_attr;
    if ((attr->equeue_size == 0u) ||
        ((attr->instance != NULL) && (attr->equeue_storage == NULL)) ||
        ((attr->instance == NULL) && (attr->equeue_storage != NULL))) {
        return EDS_ERROR__INVLD_CONFIGURATION;
    }
    if (attr->instance == NULL) {
        size_t sm_size_bytes;

        sm_size_bytes = sizeof(struct eds_object__sm) +
                eds_core__equeue__calculate_storage_size(attr->equeue_size);
        mem = eds_core__mem__select(sm_size_bytes);
        if (mem == NULL) {
            return EDS_ERROR__NO_RESOURCE;
        }
        l_sm = eds_core__mem__allocate(mem, sm_size_bytes);
        if (l_sm == NULL) {
            return EDS_ERROR__NO_MEMORY;
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
    return EDS_ERROR__NONE;
}

eds_error
eds_sm__delete(eds_sm *sm);

eds_error
eds_sm__send_signal(eds_sm * sm,
                    uint32_t event_id,
                    uint32_t timeout_ms);

eds_error
eds_sm__send_event(eds_sm *         sm,
                   const eds_event *event,
                   uint32_t         timeout_ms);

eds_error
eds_sm__send_event_after(eds_sm *         sm,
                         const eds_event *event,
                         uint32_t         after_ms);

eds_error
eds_sm__send_event_every(eds_sm *         sm,
                         const eds_event *event,
                         uint32_t         every_ms);