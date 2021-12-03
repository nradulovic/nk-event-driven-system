
#include "nk_eds.h"
#include "eds_core.h"

#include <assert.h>

size_t
eds__p__event__calculate_bundle_size(size_t event_data_size)
{
    return sizeof(struct eds_object__event) + eds_port__align_up(event_data_size);
}

struct eds_object__event *
eds__p__event__allocate(size_t                  event_data_size,
                        struct eds_object__mem *mem)
{
    struct eds_object__event *event;
    struct eds_port__critical critical;
    size_t event_size;

    event_size = eds__p__event__calculate_bundle_size(event_data_size);
    eds_port__critical__lock(&critical);
    event = eds_core__mem__allocate(mem, event_size);
    eds_port__critical__unlock(&critical);

    return event;
}

void
eds__p__event__deallocate(struct eds_object__event *event)
{
    struct eds_port__critical critical;

    eds_port__critical__lock(&critical);
    eds__p__event__ref_down(event);
    if (eds__p__event__is_in_use(event) == false) {
        /* Check if event was allocated dynamically */
        if (event->p__mem) {
            eds_core__mem__deallocate(event->p__mem, event);
        }
    }
    eds_port__critical__unlock(&critical);
}

void
eds__p__event__init(struct eds_object__event *event,
                      uint32_t                  event_id,
                      size_t                    event_data_size,
                      struct eds_object__mem *  mem)
{
    event->p__id        = event_id;
    event->p__size      = event_data_size;
    event->p__mem       = mem;
    event->p__ref_count = 0u;
}

void
eds__p__event__ref_up(struct eds_object__event * event)
{
    event->p__ref_count++;
}

void
eds__p__event__ref_down(struct eds_object__event * event)
{
    event->p__ref_count--;
}

bool
eds__p__event__is_in_use(const struct eds_object__event * event)
{
    return (event->p__ref_count == 0u);
}

eds_error
eds_event__create(uint32_t     event_id,
                  size_t       event_data_size,
                  eds_event ** event)
{
    struct eds_object__event * l_event;
    struct eds_object__mem * mem;
    size_t event_size;

    if ((event_id == 0) || (event == NULL)) {
        return EDS_ERROR__INVLD_ARGUMENT;
    }
    event_size = eds__p__event__calculate_bundle_size(event_data_size);
    mem = eds_core__mem__select(event_size);
    if (mem == NULL) {
        return EDS_ERROR__NO_RESOURCE;
    }
    l_event = eds__p__event__allocate(event_data_size, mem);
    if (l_event == NULL) {
        return EDS_ERROR__NO_MEMORY;
    }
    eds__p__event__init(l_event, event_id, event_data_size, mem);
    *event = l_event;
    return EDS_ERROR__NONE;
}

eds_error
eds_event__init(eds_event * event,
                uint32_t    event_id,
                size_t      event_data_size)
{
    if ((event_id == 0) || (event == NULL)) {
        return EDS_ERROR__INVLD_ARGUMENT;
    }
    eds__p__event__init(event, event_id, event_data_size, NULL);

    return EDS_ERROR__NONE;
}

uint32_t
eds_event__id(const eds_event * event)
{
    assert(event != NULL);
    return event->p__id;
}

void *
eds_event__data(eds_event * event)
{
    assert(event != NULL);
    if (event->p__size != 0u) {
        return event + 1u;
    } else {
        return NULL;
    }
}

size_t
eds_event__size(const eds_event * event)
{
    assert(event != NULL);
    return event->p__size;
}
