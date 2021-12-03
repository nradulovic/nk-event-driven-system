
#include "eds_core.h"

extern inline void
eds_core__list__init(struct eds_object__list * self);

extern inline struct eds_object__list*
eds_core__list__next(struct eds_object__list * self);

extern inline struct eds_object__list*
eds_core__list__prev(struct eds_object__list * self);

extern inline void
eds_core__list__add_after(struct eds_object__list * self,
                          struct eds_object__list * after);

extern inline void
eds_core__list__add_before(struct eds_object__list * self,
                           struct eds_object__list * before);

extern inline void
eds_core__list__remove(struct eds_object__list * self);

extern inline bool
eds_core__list__is_empty(const struct eds_object__list * self);

static size_t
calculate_event_size(size_t event_data_size)
{
    return sizeof(struct eds_object__event) + eds_port__align_up(event_data_size);
}


struct eds_object__event *
eds_core__event__create(uint32_t event_id,
                        size_t event_data_size,
                        struct eds_object__mem * mem)
{
    struct eds_object__event * event;

    event = eds_core__mem__allocate(mem, calculate_event_size(event_data_size));
    if (event != NULL) {
        eds_core__event__init(event, event_id, event_data_size, mem);
    }

    return event;
}

void eds_core__event__init(struct eds_object__event * event,
                           uint32_t event_id,
                           size_t event_data_size,
                           struct eds_object__mem * mem)
{
    event->p__id = event_id;
    event->p__size = event_data_size;
    event->p__mem = mem;
    event->p__ref_count = 0u;
}
