
#include "nk_eds.h"
#include "nk_eds_object.h"
#include "eds_core.h"
#include "nk_eds_port.h"

static size_t
calculate_event_size(size_t event_data_size)
{
    return sizeof(struct eds_object__event) + eds_port__align_up(event_data_size);
}

eds_error
eds_event__create(uint32_t event_id,
                  size_t event_data_size,
                  eds_event ** event)
{
    struct eds_object__event * l_event;
    struct eds_object__mem * mem;
    size_t event_size;

    if ((event_id == 0) || (event == NULL)) {
        return EDS_ERROR__INVLD_ARGUMENT;
    }
    event_size = calculate_event_size(event_data_size);
    mem = eds_core__mem__select(event_size);
    if (mem == NULL) {
        return EDS_ERROR__NO_RESOURCE;
    }
    l_event = eds_core__event__create(event_id, event_data_size, mem);
    if (l_event == NULL) {
        return EDS_ERROR__NO_MEMORY;
    }
    *event = l_event;
    return EDS_ERROR__NONE;
}

uint32_t eds_event__id(const eds_event * event)
{
    return eds_core__event__id(event);
}

void * eds_event__data(eds_event * event)
{
    return eds_core__event__data(event);
}

size_t eds_event__size(const eds_event * event)
{
    return eds_core__event__size(event);
}
