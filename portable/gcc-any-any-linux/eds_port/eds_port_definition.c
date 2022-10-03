/*
 * eds_port_definition.c
 *
 *  Created on: Dec 14, 2021
 *      Author: nenad
 */

#if __STDC_VERSION__ >= 199901L
#define _POSIX_C_SOURCE 200809L
#endif

#include "eds_port.h"
#include "eds.h"

#include <semaphore.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>

static pthread_mutex_t s__critical_mutex;

struct tick_context
{
    struct sigaction sigaction;
    pthread_t timer_thread;
    sem_t timer_lock;
};

static struct tick_context tick;

#define ALIGN_UP(num, align)                                       \
        (((num) + (align) - 1u) & ~((align) - 1u))

static void
critical_mutex_init(void)
{
    pthread_mutexattr_t mutextattr;
    int error;

    error = pthread_mutexattr_init(&mutextattr);
    assert(error == 0);
    error = pthread_mutexattr_settype(&mutextattr, PTHREAD_MUTEX_RECURSIVE);
    assert(error == 0);
    error = pthread_mutex_init(&s__critical_mutex, NULL);
    assert(error == 0);
}

static void
timer_handler(int sig_no)
{
    int error;

    (void)sig_no;
    error = sem_post(&tick.timer_lock);
    assert(error == 0);
}

static void*
tick_thread(void * arg)
{
    (void)arg;

    for (;;) {
        eds__error error;

        sem_wait(&tick.timer_lock);
        error = eds__tick_process_all();
        assert(error == EDS__ERROR_NONE);
    }
    return NULL;
}

void
eds_port__critical_local_lock(struct eds_port__critical_local * critical)
{
    int error;

    (void)critical;
    error = pthread_mutex_lock(&s__critical_mutex);
    assert(error == 0);
}

void
eds_port__critical_local_unlock(struct eds_port__critical_local * critical)
{
    int error;

    (void)critical;
    error = pthread_mutex_unlock(&s__critical_mutex);
    assert(error == 0);
}


void
eds_port__sleep_local_init(struct eds_port__sleep_local * sleep)
{
    int error;

    error = sem_init(&sleep->semaphore, 0, 0);
    assert(error == 0);
}

void
eds_port__sleep_local_wait(struct eds_port__sleep_local * sleep)
{
    int error;

    error = sem_wait(&sleep->semaphore);
    assert((error == 0) || (errno == EINTR));
}

void
eds_port__sleep_local_signal(struct eds_port__sleep_local * sleep)
{
    int error;

    error = sem_post(&sleep->semaphore);
    assert(error == 0);
}

uint_fast8_t
eds_port__ffs(uint32_t value)
{
    return (uint_fast8_t)(31u - (unsigned)__builtin_clz(value));
}

size_t
eds_port__align_up(size_t non_aligned_value)
{
    return ALIGN_UP(non_aligned_value, sizeof(void*));
}

uint32_t
eds_port__tick_duration_ms(void)
{
    return 1u;
}

uint32_t
eds_port__tick_from_ms(uint32_t ms)
{
    return ms;
}

void
eds_port__init(void)
{
    critical_mutex_init();
}

void
tick_setup(void)
{
    int error;
    struct itimerval timer;

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000;
    tick.sigaction.sa_handler = &timer_handler;

    error = sem_init(&tick.timer_lock, 0, 0);
    assert(error == 0);
    error = pthread_create(&tick.timer_thread, NULL, tick_thread, NULL);
    assert(error == 0);
    error = sigaction(SIGALRM, &tick.sigaction, NULL);
    assert(error == 0);
    error = setitimer(ITIMER_REAL, &timer, NULL);
    assert(error == 0);
}
