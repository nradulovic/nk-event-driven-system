
#include "eds_tmr.h"
#include "eds_core.h"



static struct eds_object__list g__active_timers = EDS_CORE__LIST_INITIALIZER(&g__active_timers);

static void
tmr_insert(struct eds_object__tmr * tmr)
{
    struct eds_object__list * current;

    for (EDS_CORE__LIST_EACH(current, &g__active_timers)) {
        struct eds_object__tmr * current_tmr;

        current_tmr = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list);
        if (current_tmr->p__n_rtick > tmr->p__n_rtick) {
            break;
        }
        tmr->p__n_rtick -= current_tmr->p__n_rtick;
    }
    eds_core__list_add_before(&tmr->p__list, current);
}

void
eds_tmr__init(struct eds_object__tmr * tmr, void (*fn)(struct eds_object__tmr*))
{
    eds_core__list_init(&tmr->p__list);
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
    if (!eds_core__list_is_empty(&tmr->p__list)) {
        if (eds_core__list_next(&tmr->p__list) != &g__active_timers) {
            struct eds_object__tmr * next_tmr;

            next_tmr = EDS_CORE__CONTAINER_OF(eds_core__list_next(&tmr->p__list),
                struct eds_object__tmr,
                p__list);
            next_tmr->p__n_rtick += tmr->p__n_rtick;
        }
        eds_core__list_remove(&tmr->p__list);
    }
}

bool
eds_tmr__is_running(const struct eds_object__tmr *tmr)
{
    return eds_core__list_is_empty(&tmr->p__list) == true;
}

bool
eds_tmr__are_timers_pending(void)
{
    return !eds_core__list_is_empty(&g__active_timers);
}

void
eds_tmr__process_timers(void)
{
    struct eds_object__list elapsed_timers = EDS_CORE__LIST_INITIALIZER(&elapsed_timers);

    /* See if we have any active timers */
    if (!eds_core__list_is_empty(&g__active_timers)) {
        struct eds_object__list * first;
        struct eds_object__list * iterator;
        struct eds_object__list * current;

        first = eds_core__list_next(&g__active_timers);
        /* Decrement the first timer relative tick */
        EDS_CORE__CONTAINER_OF(first, struct eds_object__tmr, p__list)->p__n_rtick--;
        /* Move all zero relative timers to local elapsed_timers list */
        for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &g__active_timers)) {
            struct eds_object__tmr * current_tmr;

            current_tmr = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list);
            if (current_tmr->p__n_rtick != 0u) {
                break;
            }
            eds_core__list_remove(current);
            eds_core__list_add_after(current, &elapsed_timers);
        }
        /* Execute all elapsed timers callbacks */
        for (EDS_CORE__LIST_EACH(current, &elapsed_timers)) {
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
