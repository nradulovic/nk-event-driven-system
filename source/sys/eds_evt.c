#include "eds_evt.h"

#include "sys/eds_mem.h"
#include "sys/eds_state.h"
#include "eds_port.h"
#include "eds_trace.h"

#include <stddef.h>

static size_t
eds_evt__calculate_bundle_size(size_t event_data_size)
{
    return sizeof(struct eds_object__evt) + eds_port__align_up(event_data_size);
}

void
eds_evt__deallocate(const struct eds_object__evt * event)
{
    struct eds_object__evt * d_event;

    d_event = eds_evt__to_dynamic(event);
    if ((d_event != NULL) && (d_event->p__ref_count == 0u)) {
        struct eds_object__mem * mem;

        mem = eds_evt__mem(d_event);
        eds_evt__term(d_event);
        eds_mem__deallocate_to(mem, d_event);
    }
}

void
eds_evt__null(const struct eds_object__evt * event)
{
    struct eds_object__evt * d_event;

    d_event = eds_evt__to_dynamic(event);
    if (d_event != NULL) {
        d_event->p__id = EDS__EVENT__NULL;
        d_event->p__size = 0u;
    }
}

void
eds_evt__init(struct eds_object__evt * event,
    uint32_t event_id,
    size_t event_data_size,
    struct eds_object__mem * mem)
{
    event->p__id = event_id;
    event->p__size = event_data_size;
    event->p__mem = mem;
    event->p__ref_count = 0u;
}

void
eds_evt__term(struct eds_object__evt * d_event)
{
    d_event->p__id = EDS__EVENT__NULL;
    d_event->p__size = 0u;
    d_event->p__mem = NULL;
    d_event->p__ref_count = 0u;
}

extern inline struct eds_object__evt*
eds_evt__to_dynamic(const struct eds_object__evt * event);

extern inline void
eds_evt__ref_up(const struct eds_object__evt * event);

extern inline void
eds_evt__ref_down(const struct eds_object__evt * event);

extern inline bool
eds_evt__is_in_use(const struct eds_object__evt * event);

extern inline bool
eds_evt__is_dynamic(const struct eds_object__evt * event);

extern inline struct eds_object__mem*
eds_evt__mem(const struct eds_object__evt * event);

eds__error
eds__event_create(uint32_t event_id, size_t event_data_size, eds__event ** event)
{
    struct eds_object__mem * mem;
    struct eds_object__evt * l_event;
    size_t event_size;
    EDS_PORT__CRITICAL_INSTANCE(critical);

    if ((event_id == 0) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    event_size = eds_evt__calculate_bundle_size(event_data_size);
    mem = eds_mem__find(&eds_state__mem_instances, event_size);
    if (mem == NULL) {
        return EDS__ERROR_NO_RESOURCE;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    l_event = eds_mem__allocate_from(mem, event_size);
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    if (l_event == NULL) {
        return EDS__ERROR_NO_MEMORY;
    }
    eds_evt__init(l_event, event_id, event_data_size, mem);
    *event = l_event;
    EDS_TRACE__INFO(EDS_TRACE__SOURCE_EVENT_CREATE, "id, event_id, data_size = (%p,%u,%u)", event, event_id, event_data_size);
    return EDS__ERROR_NONE;
}

eds__error
eds__event_cancel(eds__event * event)
{
    EDS_PORT__CRITICAL_INSTANCE(critical);

    if (event == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (event->p__mem == NULL) {
        return EDS__ERROR_NO_PERMISSION;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    if (eds_evt__is_in_use(event)) {
        event->p__id = EDS__EVENT__NULL;
    } else {
        eds_mem__deallocate_to(event->p__mem, event);
    }
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    EDS_TRACE__INFO(EDS_TRACE__SOURCE_EVENT_CANCEL, "%u", event->p__id);
    return EDS__ERROR_NONE;
}

eds__error
eds__event_keep(const eds__event * event)
{
    EDS_PORT__CRITICAL_INSTANCE(critical);

    if (event == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (!eds_evt__is_dynamic(event)) {
        return EDS__ERROR_NO_PERMISSION;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    eds_evt__ref_up(event);
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    EDS_TRACE__INFO(EDS_TRACE__SOURCE_EVENT_KEEP, "%p (%u, %u)", event, event->p__id, event->p__ref_count);
    return EDS__ERROR_NONE;
}

eds__error
eds__event_toss(const eds__event * event)
{
    EDS_PORT__CRITICAL_INSTANCE(critical);

    if (event == NULL) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_EVENT_TOSS, EDS__ERROR_INVALID_ARGUMENT, "event = %p", event);
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (!eds_evt__is_dynamic(event)) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_EVENT_TOSS, EDS__ERROR_NO_PERMISSION, "%p (%u, %u)", event, event->p__id, event->p__ref_count);
        return EDS__ERROR_NO_PERMISSION;
    }
    if (!eds_evt__is_in_use(event)) {
        EDS_TRACE__EXIT(EDS_TRACE__SOURCE_EVENT_TOSS, EDS__ERROR_NO_RESOURCE, "%p (%u, %u)", event, event->p__id, event->p__ref_count);
        return EDS__ERROR_NO_RESOURCE;
    }
    EDS_PORT__CRITICAL_LOCK(&critical);
    eds_evt__ref_down(event);
    eds_evt__deallocate(event);
    EDS_PORT__CRITICAL_UNLOCK(&critical);
    EDS_TRACE__INFO(EDS_TRACE__SOURCE_EVENT_TOSS, "%p (%u, %u)", event, event->p__id, event->p__ref_count);
    return EDS__ERROR_NONE;
}

eds__error
eds__event_init(eds__event * event, uint32_t event_id, size_t event_data_size)
{
    if ((event_id == 0) || (event == NULL)) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    eds_evt__init(event, event_id, event_data_size, NULL);

    return EDS__ERROR_NONE;
}

void*
eds__event_put_data(eds__event * event)
{
    if (event->p__size != 0u) {
        return event + 1u;
    } else {
        return NULL;
    }
}

uint32_t
eds__event_id(const eds__event * event)
{
    return event->p__id;
}

const void*
eds__event_data(const eds__event * event)
{
    if (event->p__size != 0u) {
        return event + 1u;
    } else {
        return NULL;
    }
}

size_t
eds__event_size(const eds__event * event)
{
    return event->p__size;
}

