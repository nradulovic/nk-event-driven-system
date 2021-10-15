/*
 * nk_tasker.c
 *
 *  Created on: May 8, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 */


#include <stddef.h>
#include <stdint.h>

#include "generic/common/nk_debug.h"
#include "generic/tasker/nk_tasker.h"
#include "generic/portable/nk_core.h"
#include "generic/portable/nk_cpu.h"

static void 
nk_tasker__queue__initialize(struct nk_tasker__queue * self);
static void
nk_tasker__queue__move_one(struct nk_tasker__queue * self, struct nk_tasker__queue * other, uint_fast32_t id);
static void 
nk_tasker__queue__move_high(struct nk_tasker__queue * self, struct nk_tasker__queue * other);
static void 
nk_tasker__queue__move_all(struct nk_tasker__queue * self, struct nk_tasker__queue * other);
static uint32_t 
nk_tasker__queue__get(const struct nk_tasker__queue * self);
static void
nk_tasker__queue__clear(struct nk_tasker__queue * self, uint32_t id);
static uint32_t 
nk_tasker__queue__get_and_clear(struct nk_tasker__queue * self);
static void 
nk_tasker__queue__set(struct nk_tasker__queue * self, uint32_t id);

static void
tasker__switch__conditional(struct nk_tasker * self);
static void
tasker__switch(struct nk_tasker * self);

void nk_tasker__initialize(struct nk_tasker * self)
{
    self->p__current = NULL;
    for (uint32_t i = 0u; i < NK_FARRAY__LENGTH(self->p__tasks); i++) {
        nk_tasker__queue__initialize(&instances->items[i].p__ready);
    }
}

void
nk_tasker__start(struct nk_tasker * self, struct nk_task *task)
{
    nk_assert(self);
    nk_assert(task);
    nk_assert(task->p__prio <= NK_TASKER__MAX_PRIO);
    nk_assert(task->p__method != NULL);
}

void
nk_tasker__yield(struct nk_tasker * self)
{
	struct nk_cpu__isr isr;
	
	nk_assert(self);

	nk_cpu__isr__disable(&isr);
	tasker__switch__conditional(self);
	nk_cpu__isr__enable(&isr);
}

void
nk_tasker__disable(struct nk_tasker * self)
{
    nk_assert(self);
}

void
nk_tasker__enable(struct nk_tasker * self)
{
    nk_assert(self);
}

void
nk_task__initialize(struct nk_task * self)
{
    nk_assert(self);
}

void
nk_completion__initialize(struct nk_completion * self, struct nk_tasker * tasker)
{
    self->p__tasker = tasker;
	nk_tasker__queue__initialize(&self->p__blocked);
}

void
nk_completion__signal(struct nk_completion * self)
{
	struct nk_cpu__isr isr;
	
	nk_cpu__isr__disable(&isr);
	nk_tasker__queue__move_all(&self->p__blocked, &self->p__tasker->p__ready);
	tasker__switch__conditional(self->p__tasker);
	nk_cpu__isr__enable(&isr);
}

void
nk_completion__wait(struct nk_completion * self)
{
	struct nk_cpu__isr isr;
	
	nk_assert(self);
	nk_cpu__isr__disable(&isr);
	nk_tasker__queue__move_one(/* self */   &self->p__blocked,
	                           /* other */  &self->p__tasker->p__ready,
	                           /* id    */  self->p__tasker->p__current->p__prio);
	tasker__switch(self->p__tasker);
	nk_cpu__isr__enable(&isr);
}

void
nk_semaphore__initialize(struct nk_semaphore * self, int32_t count, struct nk_tasker * tasker)
{
	self->p__count = count;
	self->p__tasker= tasker;
	nk_tasker__queue__initialize(&self->p__blocked);
}

void
nk_semaphore__signal(struct nk_semaphore * self)
{
	struct nk_cpu__isr isr;
	
	nk_cpu__isr__disable(&isr);
	self->p__count++;
	nk_tasker__queue__move_high(&self->p__tasker->p__ready, &self->p__blocked);
	tasker__switch__conditional(self->p__tasker);
	nk_cpu__isr__enable(&isr);
}

void
nk_semaphore__wait(struct nk_semaphore * self)
{
	struct nk_cpu__isr isr;
	
	nk_cpu__isr__disable(&isr);
	if (self->p__count <= 0) {
		nk_tasker__queue__move_one(/* self  */  &self->p__blocked,
		                           /* other */  &self->p__tasker->p__ready,
		                           /* id    */  self->p__tasker->p__current->p__prio);
		tasker__switch(self->p__tasker);
	}
	self->p__count--;
	nk_cpu__isr__enable(&isr);
}

static void 
nk_tasker__queue__initialize(struct nk_tasker__queue * self)
{
	self->bitmap = 0u;
}

static void
nk_tasker__queue__move_one(struct nk_tasker__queue * self, 
						   struct nk_tasker__queue * other, 
						   uint_fast32_t id)
{
	uint32_t mask = 0x1u << id;
	self->bitmap |= mask;
	other->bitmap &= ~mask;
}

static void 
nk_tasker__queue__move_high(struct nk_tasker__queue * self, struct nk_tasker__queue * other)
{
	nk_assert(self->bitmap != 0);
	nk_tasker__queue__move_one(self, other, nk_tasker__queue__get(other));
}

static void 
nk_tasker__queue__move_all(struct nk_tasker__queue * self, struct nk_tasker__queue * other)
{
	self->bitmap |= other->bitmap;
	other->bitmap = 0u;
}

static uint32_t 
nk_tasker__queue__get(const struct nk_tasker__queue * self)
{
    nk_assert(self->bitmap != 0);
	return nk_cpu__ffs(self->bitmap);
}

static uint32_t 
nk_tasker__queue__get_and_clear(struct nk_tasker__queue * self)
{
    uint32_t retval;

    nk_assert(self->bitmap != 0);
    retval = nk_cpu__ffs(self->bitmap);
    self->bitmap &= ~(0x1u << retval);
    return retval;
}

static void
nk_tasker__queue__clear(struct nk_tasker__queue * self, uint32_t id)
{
	self->bitmap &= ~(0x1u << id);
}

static void 
nk_tasker__queue__set(struct nk_tasker__queue * self, uint32_t id)
{
	self->bitmap |= 0x1u << id;
}
