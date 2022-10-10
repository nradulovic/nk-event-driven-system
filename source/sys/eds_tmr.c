#include "sys/eds_tmr.h"
#include "sys/eds_core.h"

static void
tmr_sentinel_insert(struct eds_object__tmr_sentinel * tmr, struct eds_object__tmr_node * node)
{
    struct eds_object__list * current;
    struct eds_object__tmr_node * current_tmr;

    for (EDS_CORE__LIST_EACH(current, &tmr->p__active)) {

        current_tmr = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr_node, p__list);
        if (current_tmr->rtime_ticks > node->rtime_ticks) {
            current_tmr->rtime_ticks -= node->rtime_ticks;
            break;
        }
        node->rtime_ticks -= current_tmr->rtime_ticks;
    }
    eds_core__list_add_before(&node->p__list, current);
}

static void
tmr_sentinel_pend(struct eds_object__tmr_sentinel * tmr, struct eds_object__tmr_node * node)
{
    eds_core__list_add_after(&node->p__list, &tmr->p__pending);
    if (tmr->state != EDS_OBJECT__TMR_SENTINEL_STATE_RUNNING) {
        tmr->state = EDS_OBJECT__TMR_SENTINEL_STATE_RUNNING;
        eds_port__timer_start(&tmr->tick_timer);
    }
}

void
eds_tmr__node_init(
    struct eds_object__tmr_node * self,
    void (* fn)(struct eds_object__tmr_node *))
{
    eds_core__list_init(&self->p__list);
    self->rtime_ticks = 0u;
    self->itime_ticks = 0u;
    self->fn = fn;
}

bool
eds_tmr__node_is_running(const struct eds_object__tmr_node * self)
{
    return eds_core__list_is_empty(&self->p__list) == false;
}

bool
eds_tmr__node_is_periodic(const struct eds_object__tmr_node * self)
{
    return self->itime_ticks != 0u;
}

void
eds_tmr__start_after(struct eds_object__tmr_sentinel * self,
    struct eds_object__tmr_node * node,
    uint32_t after_ticks)
{
    node->rtime_ticks = after_ticks;
    node->itime_ticks = 0u;
    node->state = EDS_OBJECT__TMR_NODE_STATE_PENDING;
    tmr_sentinel_pend(self, node);
}

void
eds_tmr__start_every(struct eds_object__tmr_sentinel * self,
    struct eds_object__tmr_node * node,
    uint32_t every_ticks)
{
    node->rtime_ticks = every_ticks;
    node->itime_ticks = every_ticks;
    node->state = EDS_OBJECT__TMR_NODE_STATE_PENDING;
    tmr_sentinel_pend(self, node);
}

void
eds_tmr__cancel(struct eds_object__tmr_sentinel * self, struct eds_object__tmr_node * node)
{
    if (node->state == EDS_OBJECT__TMR_NODE_STATE_RUNNING) {
        /* Prevent from modifying the sentinel structure */
        if (eds_core__list_next(&node->p__list) != &self->p__active) {
            struct eds_object__tmr_node * next_tmr;

            next_tmr = EDS_CORE__CONTAINER_OF(
                eds_core__list_next(&node->p__list),
                struct eds_object__tmr_node,
                p__list);
            next_tmr->rtime_ticks += node->rtime_ticks;
        }
    }
    eds_core__list_remove(&node->p__list);
}

void
eds_tmr__init(struct eds_object__tmr_sentinel * tmr)
{
    EDS_PORT__CRITICAL_INSTANCE(critical);    
    eds_core__list_init(&tmr->p__active);
    eds_core__list_init(&tmr->p__pending);
    EDS_PORT__CRITICAL_LOCK(&critical);
    eds_port__timer_init(&tmr->tick_timer);
    tmr->state = EDS_OBJECT__TMR_SENTINEL_STATE_INITIALIZED;
    EDS_PORT__CRITICAL_UNLOCK(&critical);
}

bool
eds_tmr__are_timers_pending(const struct eds_object__tmr_sentinel * sentinel)
{
    if ((eds_core__list_is_empty(&sentinel->p__active) == false) ||
        (eds_core__list_is_empty(&sentinel->p__pending) == false)) {
        return true;
    } else {
        return false;
    }
}

void
eds_tmr__for_each_node(
    struct eds_object__tmr_sentinel * self,
    void (* map)(struct eds_object__tmr_node *, void *),
    void * arg)
{
    struct eds_object__list * iterator;
    struct eds_object__list * current;

    /* Map the active timers */
    for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &self->p__active)) {
        struct eds_object__tmr_node * current_node;

        current_node = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr_node, p__list);
        map(current_node, arg);
    }
    /* Map the pending timers */
    for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &self->p__pending)) {
        struct eds_object__tmr_node * current_node;

        current_node = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr_node, p__list);
        map(current_node, arg);
    }
}

void
eds_tmr__process_timers(struct eds_object__tmr_sentinel * tmr)
{
    struct eds_object__list elapsed_timers = EDS_CORE__LIST_INITIALIZER(&elapsed_timers);
    struct eds_object__list * iterator;
    struct eds_object__list * current;

    /* See if we have any pending timers. Move/insert them to sorted active timer list. */
    for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &tmr->p__pending)) {
        struct eds_object__tmr_node * current_node;

        eds_core__list_remove(current);
        current_node = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr_node, p__list);
        current_node->state = EDS_OBJECT__TMR_NODE_STATE_RUNNING;
        tmr_sentinel_insert(tmr, current_node);
    }
    /* See if we have any active timers */
    if (eds_core__list_is_empty(&tmr->p__active) == false) {
        struct eds_object__tmr_node * first_node;
        struct eds_object__list * first;

        first = eds_core__list_next(&tmr->p__active);
        first_node = EDS_CORE__CONTAINER_OF(first, struct eds_object__tmr_node, p__list);
        /* Decrement the first timer relative tick */
        first_node->rtime_ticks--;
        /* Move all zero relative timers to local elapsed_timers list */
        for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &tmr->p__active)) {
            struct eds_object__tmr_node * current_node;

            current_node = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr_node, p__list);
            if (current_node->rtime_ticks != 0u) {
                break;
            }
            eds_core__list_remove(current);
            eds_core__list_add_after(current, &elapsed_timers);
        }
        /* Execute all elapsed timers callbacks */
        for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &elapsed_timers)) {
            struct eds_object__tmr_node * current_node;

            current_node = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr_node, p__list);
            if (current_node->itime_ticks != 0u) {
                current_node->rtime_ticks = current_node->itime_ticks;
                eds_core__list_remove(current);
                tmr_sentinel_insert(tmr, current_node);
            } else {
                current_node->state = EDS_OBJECT__TMR_NODE_STATE_DORMANT;
            }
            current_node->fn(current_node);
        }
    }
    if (eds_core__list_is_empty(&tmr->p__active)) {
        eds_port__timer_stop(&tmr->tick_timer);
        tmr->state = EDS_OBJECT__TMR_SENTINEL_STATE_INITIALIZED;
    }
}
