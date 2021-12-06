#include "eds_event.h"

#include "eds_mem.h"
#include "eds_port.h"

#include <assert.h>
#include <stddef.h>

size_t
eds_event__calculate_bundle_size(size_t event_data_size)
{
    return sizeof(struct eds_object__event) + eds_port__align_up(event_data_size);
}

eds_event__error
eds_event__allocate(uint32_t event_id,
    size_t event_data_size,
    struct eds_object__event ** event)
{
    struct eds_object__mem * mem;
    struct eds_object__event * l_event;
    size_t event_size;

    event_size = eds_event__calculate_bundle_size(event_data_size);
    mem = eds_mem__select(event_size);
    if (mem == NULL) {
        return EDS_CORE__ERROR__NO_RESOURCE;
    }
    l_event = eds_mem__allocate_from(mem, event_size);
    if (l_event == NULL) {
        return EDS_CORE__ERROR__NO_MEMORY;
    }
    eds_event__init(l_event, event_id, event_data_size, mem);
    *event = l_event;
    return EDS_CORE__ERROR__NONE;
}

eds_event__error
eds_event__deallocate(const struct eds_object__event * event)
{
    struct eds_object__event * d_event;

    d_event = eds_event__to_dynamic(event);
    if (d_event == NULL)
    {
        return EDS_CORE__ERROR__NO_PERMISSION;
    }
    eds_event__ref_down(d_event);
    if (eds_event__is_in_use(d_event) == false)
    {
        eds_event__term(d_event);
        eds_mem__deallocate_to(eds_event__mem(d_event), d_event);
    }
    return EDS_CORE__ERROR__NONE;
}

void
eds_event__init(struct eds_object__event *event,
    uint32_t event_id,
    size_t event_data_size,
    struct eds_object__mem *mem)
{
    event->p__id = event_id;
    event->p__size = event_data_size;
    event->p__mem = mem;
    event->p__ref_count = 0u;
}

void
eds_event__term(struct eds_object__event *d_event)
{
    d_event->p__id = EDS__EVENT__NULL;
    d_event->p__size = 0u;
    d_event->p__mem = NULL;
    d_event->p__ref_count = 0u;
}

extern inline void
eds_event__ref_up(const struct eds_object__event *event);

extern inline void
eds_event__ref_down(struct eds_object__event *event);

extern inline bool
eds_event__is_in_use(const struct eds_object__event *event);

extern inline bool
eds_event__is_dynamic(const struct eds_object__event *event);

extern inline struct eds_object__mem*
eds_event__mem(const struct eds_object__event *event);

nk_eds_error
nk_eds_event__create(uint32_t event_id, size_t event_data_size, nk_eds_event **event)
{
    struct eds_object__event *l_event;
    eds_event__error core_error;

    if ((event_id == 0) || (event == NULL)) {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    core_error = eds_event__allocate(event_id, event_data_size, &l_event);
    switch (core_error) {
        case EDS_CORE__ERROR__NO_RESOURCE:
            return NK_EDS_ERROR__NO_RESOURCE;
        case EDS_CORE__ERROR__NO_MEMORY:
            return NK_EDS_ERROR__NO_MEMORY;
        default:
            break;
    }
    *event = l_event;
    return NK_EDS_ERROR__NONE;
}

nk_eds_error
nk_eds_event__cancel(nk_eds_event *event)
{
    struct eds_port__critical critical;

    if (event == NULL) {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    if (event->p__mem == NULL) {
        return NK_EDS_ERROR__NO_PERMISSION;
    }
    eds_port__critical__lock(&critical);
    if (eds_event__is_in_use(event)) {
        event->p__id = EDS__EVENT__NULL;
    } else {
        eds_mem__deallocate_to(event->p__mem, event);
    }
    eds_port__critical__unlock(&critical);

    return NK_EDS_ERROR__NONE;
}

nk_eds_error
nk_eds_event__init(nk_eds_event *event, uint32_t event_id, size_t event_data_size)
{
    if ((event_id == 0) || (event == NULL)) {
        return NK_EDS_ERROR__INVLD_ARGUMENT;
    }
    eds_event__init(event, event_id, event_data_size, NULL);

    return NK_EDS_ERROR__NONE;
}

uint32_t
nk_eds_event__id(const nk_eds_event *event)
{
    assert(event != NULL);
    return event->p__id;
}

void*
nk_eds_event__data(nk_eds_event *event)
{
    assert(event != NULL);
    if (event->p__size != 0u) {
        return event + 1u;
    } else {
        return NULL;
    }
}

size_t
nk_eds_event__size(const nk_eds_event *event)
{
    assert(event != NULL);
    return event->p__size;
}
