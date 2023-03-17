#include "sys/eds_tmr.h"
#include "sys/eds_core.h"
#include "sys/eds_state.h"
#include "eds_port.h"

static void
tmr_sentinel_init(struct eds_object__tmr_sentinel * sentinel);
static void
tmr_sentinel_insert(struct eds_object__tmr_sentinel * sentinel, struct eds_object__tmr * node);
static void
tmr_sentinel_pend(struct eds_object__tmr_sentinel * sentinel, struct eds_object__tmr * node);
static void
tmr_sentinel_for_each_node(
    const struct eds_object__tmr_sentinel * sentinel,
    void (* map)(struct eds_object__tmr *, void *),
    void * arg);
static void
tmr_sentinel_process_timers(struct eds_object__tmr_sentinel * sentinel);
static void tmr_cb(void);

static void
tmr_sentinel_init(struct eds_object__tmr_sentinel * sentinel)
{
    eds_core__list_init(&sentinel->p__active);
    eds_core__list_init(&sentinel->p__pending);
    sentinel->state = EDS_OBJECT__TMR_SENTINEL_STATE_INITIALIZED;
}

static void
tmr_sentinel_insert(struct eds_object__tmr_sentinel * sentinel, struct eds_object__tmr * node)
{
    struct eds_object__list * current;
    struct eds_object__tmr * current_tmr;

    for (EDS_CORE__LIST_EACH(current, &sentinel->p__active)) {

        current_tmr = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list);
        if (current_tmr->rtime_ticks > node->rtime_ticks) {
            current_tmr->rtime_ticks -= node->rtime_ticks;
            break;
        }
        node->rtime_ticks -= current_tmr->rtime_ticks;
    }
    eds_core__list_add_before(&node->p__list, current);
}

static void
tmr_sentinel_pend(struct eds_object__tmr_sentinel * sentinel, struct eds_object__tmr * node)
{
    eds_core__list_add_after(&node->p__list, &sentinel->p__pending);
    if (sentinel->state != EDS_OBJECT__TMR_SENTINEL_STATE_RUNNING) {
        sentinel->state = EDS_OBJECT__TMR_SENTINEL_STATE_RUNNING;
        eds_port__timer_start();
    }
}

static void
tmr_sentinel_for_each_node(
    const struct eds_object__tmr_sentinel * sentinel,
    void (* map)(struct eds_object__tmr *, void *),
    void * arg)
{
    struct eds_object__list * iterator;
    struct eds_object__list * current;

    /* Map the active timers */
    for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &sentinel->p__active)) {
        struct eds_object__tmr * current_node;

        current_node = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list);
        map(current_node, arg);
    }
    /* Map the pending timers */
    for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &sentinel->p__pending)) {
        struct eds_object__tmr * current_node;

        current_node = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list);
        map(current_node, arg);
    }
}

static void
tmr_sentinel_process_timers(struct eds_object__tmr_sentinel * sentinel)
{
    struct eds_object__list elapsed_timers = EDS_CORE__LIST_INITIALIZER(&elapsed_timers);
    struct eds_object__list * iterator;
    struct eds_object__list * current;

    /* See if we have any pending timers. Move/insert them to sorted active timer list. */
    for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &sentinel->p__pending)) {
        struct eds_object__tmr * current_node;

        eds_core__list_remove(current);
        current_node = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list);
        current_node->state = EDS_OBJECT__TMR_STATE_ACTIVE;
        tmr_sentinel_insert(sentinel, current_node);
    }
    /* See if we have any active timers */
    if (eds_core__list_is_empty(&sentinel->p__active) == false) {
        struct eds_object__tmr * first_node;
        struct eds_object__list * first;

        first = eds_core__list_next(&sentinel->p__active);
        first_node = EDS_CORE__CONTAINER_OF(first, struct eds_object__tmr, p__list);
        /* Decrement the first timer relative tick */
        first_node->rtime_ticks--;
        /* Move all zero relative timers to local elapsed_timers list */
        for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &sentinel->p__active)) {
            struct eds_object__tmr * current_node;

            current_node = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list);
            if (current_node->rtime_ticks != 0u) {
                break;
            }
            eds_core__list_remove(current);
            eds_core__list_add_after(current, &elapsed_timers);
        }
        /* Execute all elapsed timers callbacks */
        for (EDS_CORE__LIST_EACH_SAFE(current, iterator, &elapsed_timers)) {
            struct eds_object__tmr * current_node;

            current_node = EDS_CORE__CONTAINER_OF(current, struct eds_object__tmr, p__list);
            if (current_node->itime_ticks != 0u) {
                current_node->rtime_ticks = current_node->itime_ticks;
                eds_core__list_remove(current);
                tmr_sentinel_insert(sentinel, current_node);
            } else {
                current_node->state = EDS_OBJECT__TMR_STATE_DORMANT;
            }
            current_node->fn(current_node);
        }
    }
    if (eds_core__list_is_empty(&sentinel->p__active)) {
        eds_port__timer_stop();
        sentinel->state = EDS_OBJECT__TMR_SENTINEL_STATE_INITIALIZED;
    }
}

static void tmr_cb(void)
{
    struct eds_object__tmr_sentinel * sentinel = &eds_state__tmr_sentinel;
    tmr_sentinel_process_timers(sentinel);
}

void
eds_tmr__init(struct eds_object__tmr * self, void (* fn)(struct eds_object__tmr *))
{
    eds_core__list_init(&self->p__list);
    self->rtime_ticks = 0u;
    self->itime_ticks = 0u;
    self->fn = fn;
}

bool
eds_tmr__is_running(const struct eds_object__tmr * self)
{
    return eds_core__list_is_empty(&self->p__list) == false;
}

extern bool
eds_tmr__is_periodic(const struct eds_object__tmr * self);

void
eds_tmr__start_after(struct eds_object__tmr * self, uint32_t after_ticks)
{
    struct eds_object__tmr_sentinel * sentinel = &eds_state__tmr_sentinel;

    self->rtime_ticks = after_ticks;
    self->itime_ticks = 0u;
    self->state = EDS_OBJECT__TMR_STATE_PENDING;
    tmr_sentinel_pend(sentinel, self);
}

void
eds_tmr__start_every(struct eds_object__tmr * self, uint32_t every_ticks)
{
    struct eds_object__tmr_sentinel * sentinel = &eds_state__tmr_sentinel;

    self->rtime_ticks = every_ticks;
    self->itime_ticks = every_ticks;
    self->state = EDS_OBJECT__TMR_STATE_PENDING;
    tmr_sentinel_pend(sentinel, self);
}

void
eds_tmr__cancel(struct eds_object__tmr * self)
{
    struct eds_object__tmr_sentinel * sentinel = &eds_state__tmr_sentinel;

    if (self->state == EDS_OBJECT__TMR_STATE_ACTIVE) {
        /* Prevent from modifying the sentinel structure */
        struct eds_object__list * next;

        next = eds_core__list_next(&self->p__list);
        if (next != &sentinel->p__active) {
            struct eds_object__tmr * next_tmr;

            next_tmr = EDS_CORE__CONTAINER_OF(next, struct eds_object__tmr, p__list);
            next_tmr->rtime_ticks += self->rtime_ticks;
        }
    }
    eds_core__list_remove(&self->p__list);
}

void
eds_tmr__sentinel_init(void)
{
    struct eds_object__tmr_sentinel * sentinel = &eds_state__tmr_sentinel;

    tmr_sentinel_init(sentinel);
    eds_port__timer_set_cb(tmr_cb);
}

void
eds_tmr__sentinel_map_for_each_node(
    void (* map)(struct eds_object__tmr *, void *),
    void * arg)
{
    const struct eds_object__tmr_sentinel * sentinel = &eds_state__tmr_sentinel;
    
    tmr_sentinel_for_each_node(sentinel, map, arg);
}
