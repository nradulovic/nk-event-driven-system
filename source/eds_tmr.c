
#include "eds_tmr.h"
#include "eds_core.h"



static struct eds_object__list g__active_timers = EDS_CORE__LIST_INITIALIZER(&g__active_timers);

static void
tmr_insert(struct eds_object__tmr * tmr)
{
    struct eds_object__list * current_node;

    current_node = eds_core__list__next(&g__active_timers);

    while (current_node != &g__active_timers) {
        struct eds_object__tmr * current_tmr;

        current_tmr = EDS_CORE__CONTAINER_OF(current_node, struct eds_object__tmr, p__list);
        if (current_tmr->p__n_rtick > tmr->p__n_rtick) {
            break;
        }
        tmr->p__n_rtick -= current_tmr->p__n_rtick;
        current_node = eds_core__list__next(current_node);
    }
    eds_core__list__add_before(&tmr->p__list, current_node);
}

void
eds_tmr__init(struct eds_object__tmr * tmr, void (*fn)(struct eds_object__tmr*))
{
    eds_core__list__init(&tmr->p__list);
    tmr->p__n_rtick = 0u;
    tmr->p__n_itick = 0u;
    tmr->p__fn = fn;
}

void
eds_tmr__start_once(struct eds_object__tmr *tmr, uint32_t ticks)
{
    ticks++;
    tmr->p__n_rtick = ticks;
    tmr->p__n_itick = 0u;
    tmr_insert(tmr);
}

void
eds_tmr__start_periodic(struct eds_object__tmr *tmr, uint32_t ticks)
{
    ticks++;
    tmr->p__n_rtick = ticks;
    tmr->p__n_itick = ticks;
    tmr_insert(tmr);
}

void
eds_tmr__cancel(struct eds_object__tmr *tmr)
{
    if (!eds_core__list__is_empty(&tmr->p__list)) {
        if (eds_core__list__next(&tmr->p__list) != &g__active_timers) {
            struct eds_object__tmr * next_tmr;

            next_tmr = EDS_CORE__CONTAINER_OF(eds_core__list__next(&tmr->p__list),
                struct eds_object__tmr,
                p__list);
            next_tmr->p__n_rtick += tmr->p__n_rtick;
        }
        eds_core__list__remove(&tmr->p__list);
    }
}

bool
eds_tmr__is_running(const struct eds_object__tmr *tmr)
{
    return eds_core__list__is_empty(&tmr->p__list) == true;
}

void
eds_tmr__process_tick(void)
{
    struct eds_object__list * current;
    struct eds_object__list * iterator;
    struct eds_object__list elapsed = EDS_CORE__LIST_INITIALIZER(&elapsed);

    current = eds_core__list__next(&g__active_timers);
    /* Decrement the first timer relative tick */
    if (current != &g__active_timers) {
        EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list)->p__n_rtick--;
        /* Move all zero relative timers to local elapsed list */
        for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &g__active_timers)) {
            struct eds_object__tmr * current_tmr;

            current_tmr = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list);
            if (current_tmr->p__n_rtick != 0u) {
                break;
            }
            eds_core__list__remove(current);
            eds_core__list__add_after(current, &elapsed);
        }
        /* Execute all elapsed timers callbacks */
        for (EDS_CORE__LIST_EACH(current, &elapsed)) {
            struct eds_object__tmr * current_tmr;

            current_tmr = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list);
            if (current_tmr->p__n_itick != 0u) {
                current_tmr->p__n_rtick = current_tmr->p__n_itick;
                tmr_insert(current_tmr);
            }
            current_tmr->p__fn(current_tmr);
        }
    }
}
