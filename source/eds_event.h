/*
 * eds_event.h
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_EDS_EVENT_H_
#define NEON_KIT_GENERIC_SOURCE_EDS_EVENT_H_

#include "eds_object.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

size_t
eds_event__calculate_bundle_size(size_t event_data_size);

void
eds_event__init(struct eds_object__event *event,
    uint32_t event_id,
    size_t event_data_size,
    struct eds_object__mem *mem);

void
eds_event__term(const struct eds_object__event *event);

inline void
eds_event__ref_up(struct eds_object__event *event)
{
    event->p__ref_count++;
}

inline void
eds_event__ref_down(struct eds_object__event *event)
{
    event->p__ref_count--;
}

inline bool
eds_event__is_in_use(const struct eds_object__event *event)
{
    return (event->p__ref_count == 0u);
}

inline bool
eds_event__is_dynamic(const struct eds_object__event *event)
{
    return (event->p__mem != NULL);
}

inline struct eds_object__mem*
eds_event__mem(const struct eds_object__event *event)
{
    return event->p__mem;
}

inline struct eds_object__event *
eds_event__to_dynamic(const struct eds_object__event * event)
{
    if (event->p__mem != NULL) {
        /*
         * At this point we know that event is dynamically allocated, therefore, we are safe to
         * discard pointer const-ness.
         */
        return (struct eds_object__event *)(uintptr_t)event;
    } else {
        return NULL;
    }
}

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_EVENT_H_ */
