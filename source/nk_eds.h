/*
 * nk_eds.h
 *
 *  Created on: Dec 2, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_NK_EDS_H_
#define NEON_KIT_GENERIC_SOURCE_NK_EDS_H_

#include <stdint.h>
#include <stddef.h>

typedef struct eds_object__epa eds_epa;
typedef struct eds_object__sm eds_sm;
typedef struct eds_object__event eds_event;
typedef uint32_t eds_error;

#define EDS_ERROR__NONE                     0
#define EDS_ERROR__INVLD_ARGUMENT           0x01
#define EDS_ERROR__NO_MEMORY                0x02
#define EDS_ERROR__NO_RESOURCE              0x03

/**
 * @brief       Add memory allocator for EDS objects creation handling.
 *
 * This functions needs to be called before any other function from EDS package. It will store
 * the information about available allocators. Allocators are used by create functions of EDS.
 *
 * If no allocators are provided EDS will use standard C library malloc and free. This might not be desirable
 * so by adding new memory allocators standard C allocators wont be used.
 *
 * Specify for each allocator malloc/free pair how big memory partitions it can handle. This adds
 * ability to use multiple memory pools of different sizes.
 *
 * EDS can accept up to 32 different allocators. It was shown in practice that up to 7 allocators of different
 * size are needed for a typical application.
 *
 * @param       alloc is a pointer to function which allocates memory space. This function receives only one argument
 *              @a size which tells to allocator how big the memory block should be.
 * @param       dealloc is a pointer to function which receives the previosly allocated block and it should recycle its
 *              space.
 * @param       context is pointer to allocator context structure if it needed by allocator.
 * @param       max_size is maximum size of block that can be allocated and freed by allocator. In case when a pool memory
 *              is used this argument will be equal to pool  memory block size. When this argument is set to zero or
 *              SIZE_MAX value then this allocator would be used in case no other allocator sattisifes required size.
 *
 * @return      Operation status.
 * @retval      EDS_ERROR__NONE Operation completed successfully.
 * @retval      EDS_ERROR__INVLD_ARGUMENT Is returned when either @a alloc or @dealloc arguments are NULL pointer
 * @retval      EDS_ERROR__NO_RESOURCE Is returned when application tried to add more than 32 instances of memory
 *              allocator.
 */
eds_error
eds__add_mem(void* (*alloc)(void *, size_t size),
                   void  (*dealloc)(void*, void*),
                   void * context,
                   size_t max_size);

eds_error
eds_event__create(uint32_t event_id, size_t event_data_size, eds_event ** event);

uint32_t eds_event__id(const eds_event * event);
void * eds_event__data(eds_event * event);
size_t eds_event__size(const eds_event * event);

typedef enum eds_sm__action
{
    EDS_SM__ACTION__SUPER,
    EDS_SM__ACTION__HANDLED,
    EDS_SM__ACTION__IGNORED,
    EDS_SM__ACTION__PUSHED_BACK,
    EDS_SM__ACTION__TRANSIT
} eds_sm__action;

typedef eds_sm__action
(eds_sm__state_fn)(eds_sm*, void *workspace, const eds_event *event);

typedef uint32_t eds_sm__prio;

struct eds_sm__attr
{
    const char *name;
    eds_sm__prio prio;
    eds_sm *instance;
    size_t equeue_size;
    eds_event *equeue_storage;
};

eds_error
eds_sm__create(eds_sm__state_fn *initial_state, void *sm_workspace, const struct eds_sm__attr *attr, eds_sm **sm);

eds_error
eds_sm__delete(eds_sm *sm);

eds_error
eds_sm__send_signal(eds_sm * sm, uint32_t event_id, uint32_t timeout_ms);

eds_error
eds_sm__send_event(eds_sm *sm, const eds_event *event, uint32_t timeout_ms);

eds_error
eds_sm__send_event_after(eds_sm *sm, const eds_event *event, uint32_t after_ms);

eds_error
eds_sm__send_event_every(eds_sm *sm, const eds_event *event, uint32_t every_ms);

eds_epa*
eds_sm__get_epa(const eds_sm *sm);

struct eds_epa__attr
{
    const char *name;
    eds_epa *instance;
};

eds_error
eds_epa__create(const eds_epa__attr *atrr, eds_epa **epa);

eds_error
eds_epa__delete(eds_epa *epa);

eds_error
eds_epa__add(eds_epa *epa, eds_sm *sm);

eds_error
eds_epa__remove(eds_epa *epa, eds_sm *sm);

eds_error
eds_epa__start_all(eds_epa *epa);

eds_error
eds_epa__stop_all(eds_epa *epa);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_H_ */
