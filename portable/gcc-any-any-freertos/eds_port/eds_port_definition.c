/*
 * eds_port_definition.c
 *
 *  Created on: Dec 14, 2021
 *      Author: nenad
 */

#include "eds_port_definition.h"
#include "eds_port_config_definition.h"

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#define TIMER_PERIOD_TICKS          pdMS_TO_TICKS(EDS_PORT_CONFIG__TICK_PERIOD_MS)

struct eds_port__timer
{
    TimerHandle_t instance;
    StaticTimer_t buffer;
    void (* tmr_callback)(void);
};

static struct eds_port__timer global_timer;

struct eds_port__critical eds_port__global_critical;

#define ALIGN_UP(num, align)                                       \
        (((num) + (align) - 1u) & ~((align) - 1u))

void
eds_port__critical_lock(struct eds_port__critical * critical)
{
    xSemaphoreTakeRecursive(critical->instance, portMAX_DELAY);
}

void
eds_port__critical_unlock(struct eds_port__critical * critical)
{
    xSemaphoreGiveRecursive(critical->instance);
}

void
eds_port__sleep_init(struct eds_port__sleep * sleep)
{
    sleep->instance = xSemaphoreCreateBinaryStatic(&sleep->buffer);
    assert(sleep->instance);
}

void
eds_port__sleep_wait(struct eds_port__sleep * sleep)
{
    xSemaphoreTake(sleep->instance, portMAX_DELAY);
}

void
eds_port__sleep_signal(struct eds_port__sleep * sleep)
{
    xSemaphoreGive(sleep->instance);
}

uint_fast8_t
eds_port__ffs(uint32_t value)
{
    return (uint_fast8_t)(31u - (unsigned)__builtin_clz(value));
}

uint32_t
eds_port__tick_duration_ms(void)
{
    return EDS_PORT_CONFIG__TICK_PERIOD_MS;
}

uint32_t
eds_port__tick_from_ms(uint32_t ms)
{
    return pdMS_TO_TICKS(ms);
}

static void timer_callback(TimerHandle_t os_timer)
{
    struct eds_port__timer * timer = pvTimerGetTimerID(os_timer);
    if (timer->tmr_callback != NULL) {
        timer->tmr_callback();
    }
}

void
eds_port__timer_set_cb(void (* callback)(void))
{
    struct eds_port__timer * timer = &global_timer;
    timer->tmr_callback = callback;
}

void
eds_port__timer_start(void)
{
    struct eds_port__timer * timer = &global_timer;
    xTimerStart(timer->instance, portMAX_DELAY);
}

void
eds_port__timer_stop(void)
{
    struct eds_port__timer * timer = &global_timer;
    xTimerStop(timer->instance, portMAX_DELAY);
}

void
eds_port__init(void)
{
    struct eds_port__timer * timer = &global_timer;

    eds_port__global_critical.instance = xSemaphoreCreateRecursiveMutexStatic(&eds_port__global_critical.buffer);
    assert(eds_port__global_critical.instance);
    timer->instance = xTimerCreateStatic("EDS timer", TIMER_PERIOD_TICKS, pdTRUE, timer, timer_callback, &timer->buffer);
    assert(timer->instance);
}
