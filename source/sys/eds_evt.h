/*
 * eds_event.h
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_EDS_EVENT_H_
#define NEON_KIT_GENERIC_SOURCE_EDS_EVENT_H_

#include "eds_object.h"
#include "sys/eds_core.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define EDS_EVT__INITIALIZER(event_id, event_data_size)                                         \
        {                                                                                       \
            .p__id = (event_id),                                                                \
            .p__size = (event_data_size),                                                       \
        }

void
eds_evt__deallocate(const struct eds_object__evt * event);

void
eds_evt__null(const struct eds_object__evt * event);

void
eds_evt__init(struct eds_object__evt * event,
    uint32_t event_id,
    size_t event_data_size,
    struct eds_object__mem * mem);

#define eds_evt__is_dynamic(event)          ((event)->p__mem != NULL)
#define eds_evt__is_in_use(event)           ((event)->p__ref_count != 0u)

inline struct eds_object__evt*
eds_evt__to_dynamic(const struct eds_object__evt * event)
{
    return (struct eds_object__evt*)(uintptr_t)event;
}

inline void
eds_evt__ref_up(const struct eds_object__evt * event)
{
    if (eds_evt__is_dynamic(event) && (event->p__ref_count < UINT32_MAX)) {
        struct eds_object__evt * d_event;

        d_event = eds_evt__to_dynamic(event);
        d_event->p__ref_count++;
    }
}

inline void
eds_evt__ref_down(const struct eds_object__evt * event)
{
    if (eds_evt__is_dynamic(event) && (event->p__ref_count > 0u)) {
        struct eds_object__evt * d_event;

        d_event = eds_evt__to_dynamic(event);
        d_event->p__ref_count--;
    }
}

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_EVENT_H_ */
