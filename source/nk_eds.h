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
#include <stdbool.h>

typedef struct eds_object__epa eds_epa;
typedef struct eds_object__sm nk_eds_epa;
typedef struct eds_object__event nk_eds_event;
typedef uint32_t nk_eds_error;

#define EDS__ALLOCATOR__N_OF_INSTANCES      16u
#define EDS__ALLOCATOR__MIN_BYTES           16u

#define EDS__SM__DEFAULT_QUEUE_N_OF_ENTRIES 16u
#define EDS__EPA__DEFAULT_PRIO              16u
#define EDS__EPA__DEFAULT_NAME              "nameless"

#define NK_EDS_ERROR__NONE                  0
#define NK_EDS_ERROR__INVLD_ARGUMENT        0x01
#define NK_EDS_ERROR__NO_MEMORY             0x02
#define NK_EDS_ERROR__NO_RESOURCE           0x03
#define NK_EDS_ERROR__NO_PERMISSION         0x04
#define NK_EDS_ERROR__EXISTS                0x05
#define NK_EDS_ERROR__INVLD_CONFIGURATION   0x06
#define NK_EDS_ERROR__INVLD_USAGE           0x07


typedef void*
(eds__alloc__fn)(void*, size_t);
typedef void
(eds__dealloc__fn)(void*, void*);

/**
 * @brief       Add memory allocator for EDS objects creation.
 *
 * This functions needs to be called before any other function from EDS package. The function will
 * store the information about available allocators. Allocators are used by create functions of EDS
 * when creating new objects. All objects used by EDS may be allocated dynamically or statically.
 * In order to create a static object use attribute structure `instance` pointer.
 *
 * If no allocators are provided EDS will not be able to create new objects and errors will be
 * returned.
 *
 * Specify for each allocator malloc/free pair how big memory partitions it can handle. This adds
 * ability to use multiple memory pools of different sizes.
 *
 * EDS can accept up to @ref EDS__ALLOCATOR__N_OF_INSTANCES different allocators. It was shown in
 * practice that around 7 allocators of different sizes are needed for a typical application.
 *
 * @param       alloc is a pointer to function which allocates memory space. Allocator function
 *              receives the following arguments: first is the pointer to context structure, which
 *              was provided by @a context argument of this function; the second second argument
 *              tells to allocator function how big the memory block should be allocated. The size
 *              of this argument can be anything between zero and @a max_size argument of this
 *              function. The allocator function returns `void` pointer to allocated memory. If it
 *              can not allocate memory it returns NULL pointer. Allocator function does not need
 *              handle zero allocations (size of allocation is equal to zero).
 * @param       dealloc is a pointer to function which receives the context structure and the
 *              previously allocated block and it should recycle its space. This function does not
 *              need to handle NULL deallocations.
 * @param       context is pointer to allocator context structure if it needed by allocator. The
 *              structure needs to exist the whole time while allocator functions are being used.
 * @param       max_size is maximum size of block that can be allocated and freed by allocator. In
 *              case when a pool memory is used this argument will be equal to pool  memory block
 *              size. When this argument is set to zero or SIZE_MAX value then this allocator would
 *              be used in case no other allocator satisfies required size.
 *
 * @return      Operation status.
 * @retval      EDS_ERROR__NONE Operation completed successfully.
 * @retval      EDS_ERROR__INVLD_ARGUMENT Is returned when either @a alloc or @a dealloc arguments
 *              are NULL pointer or when @a max_size is less than @ref EDS__ALLOCATOR__MIN_BYTES
 *              bytes.
 * @retval      EDS_ERROR__NO_RESOURCE Is returned when application tried to add more than
 *              @ref EDS__ALLOCATOR__N_OF_INSTANCES instances of memory allocator.
 * @retval      EDS_ERROR__EXISTS The allocator with @a max_size memory block was already added.
 */
nk_eds_error
nk_eds_mem__add_allocator(eds__alloc__fn *alloc,
    eds__dealloc__fn *dealloc,
    void *context,
    size_t max_size);

/**
 * @brief       Create an event and initialize it.
 *
 * Before an event can be generated it needs to be created. This function creates a dynamic event.
 * Dynamic event are events that are allocated using a memory allocator.
 *
 * @param       event_id Event identification number. This is a unique number which identifies an
 *              event. Event identifiers are usually handled by enumerations. It is application
 *              responsibility to allocate unique numbers for each event.
 * @param       event_data_size Some events might have some attached data. This argument specifies
 *              the size of attached data in bytes. When event does not have any data put this
 *              argument to zero.
 * @param [out] event Pointer to `eds_event *` variable. This variable will hold the pointer to
 *              newly created event. The application might want to attach data to this event (see
 *              @ref eds_event__data function).
 * @return      Operation status.
 * @retval      EDS_ERROR__NONE Operation completed successfully.
 * @retval      EDS_ERROR__INVLD_ARGUMENT Is returned when either @a event_id is equal to zero or
 *              when @a event pointer is set to NULL.
 * @retval      EDS_ERROR__NO_RESOURCE When no suitable allocator was added with
 *              @ref eds__add_allocator function and EDS is then unable to allocate memory space for
 *              the event.
 * @retval      EDS_ERROR__NO_MEMORY When a suitable allocator was found but its memory reserves
 *              have depleted.
 */
nk_eds_error
nk_eds_event__create(uint32_t event_id, size_t event_data_size, nk_eds_event **event);

nk_eds_error
nk_eds_event__cancel(nk_eds_event *event);

/**
 * @brief       Initialize a static event.
 *
 * Static event is an event which always exists. Once processed it can not be deleted. Using static
 * events is an alternative when embedded system does not allow any memory allocation.
 *
 * @param       event Is a pointer to statically allocated event structure. The definition of this
 *              structure is available in `eds_object.h` header file.
 * @param       event_id Event identification number. This is a unique number which identifies an
 *              event. Event identifiers are usually handled by enumerations. It is application
 *              responsibility to allocate unique numbers for each event.
 * @param       event_data_size Some events might have some attached data. This argument specifies
 *              the size of attached data in bytes. When event does not have any data put this
 *              argument to zero.
 * @return      Operation status.
 * @retval      EDS_ERROR__NONE Operation completed successfully.
 * @retval      EDS_ERROR__INVLD_ARGUMENT Is returned when either @a event_id is equal to zero or
 *              when @a event pointer is set to NULL.
 */
nk_eds_error
nk_eds_event__init(nk_eds_event *event, uint32_t event_id, size_t event_data_size);

/**
 * @brief       Get event identification number from the event.
 *
 * @pre         Argument @a event must be a non-NULL pointer and pointing to a previously initialized
 *              or created event.
 * @param       event Pointer to event.
 * @return      Event identification number (uint32_t).
 */
uint32_t
nk_eds_event__id(const nk_eds_event *event);

/**
 * @brief       Get attached data from the event.
 *
 * @pre         Argument @a event must be a non-NULL pointer and pointing to a previously initialized
 *              or created event.
 * @param       event Pointer to event.
 * @return      Pointer to event data.
 * @retval      NULL The event has no attached data.
 */
void*
nk_eds_event__data(nk_eds_event *event);

/**
 * @brief       Get event attached data size from the event.
 *
 * @pre         Argument @a event must be a non-NULL pointer and pointing to a previously initialized
 *              or created event.
 * @param       event Pointer to event.
 * @return      Size of event attached data in bytes.
 * @retval      0 The event has no attached data.
 */
size_t
nk_eds_event__size(const nk_eds_event *event);

struct eds_object__mem;
typedef struct eds_object__sm nk_eds_sm;

/**
 * @brief       Returned action enumerator
 *
 * The enumerator defines what action should state machine dispatcher execute for the given state
 * machine.
 */
typedef enum eds_sm__action
{
    EDS_SM__ACTION__SUPER,
    EDS_SM__ACTION__HANDLED,
    EDS_SM__ACTION__IGNORED,
    EDS_SM__ACTION__PUSHED_BACK,
    EDS_SM__ACTION__TRANSIT
} nk_eds_sm__action;

#define EDS__EVENT__INIT                    1
#define EDS__EVENT__ENTER                   2
#define EDS__EVENT__EXIT                    3
#define EDS__EVENT__SUPER                   4
#define EDS__EVENT__NULL                    0
#define EDS__EVENT__USER                    64

typedef nk_eds_sm__action
(nk_eds_sm__state_fn)(nk_eds_sm*, void *workspace, const nk_eds_event *event);

typedef uint_fast8_t nk_eds_epa__prio;

struct nk_eds_epa__attr
{
    const char *name;
    nk_eds_epa__prio prio;
    nk_eds_epa *instance;
    size_t equeue_size;
    nk_eds_event *equeue_storage;
};

/**
 * @brief       Create and initialize a state machine.
 *
 * @param       initial_state
 * @param       sm_workspace
 * @param       attr
 * @param [out] sm
 * @return eds_error
 */
nk_eds_error
nk_eds_epa__create(nk_eds_sm__state_fn *initial_state,
    void *sm_workspace,
    const struct nk_eds_epa__attr *attr,
    nk_eds_epa **sm);

nk_eds_error
eds_sm__delete(nk_eds_epa *sm);

nk_eds_error
eds_sm__send_signal(nk_eds_epa *sm, uint32_t event_id);

nk_eds_error
nk_eds_sm__send_event(nk_eds_epa *sm, const nk_eds_event *event);

nk_eds_error
eds_sm__send_event_after(nk_eds_epa *sm, const nk_eds_event *event, uint32_t after_ms);

nk_eds_error
eds_sm__send_event_every(nk_eds_epa *sm, const nk_eds_event *event, uint32_t every_ms);

eds_epa*
eds_sm__get_epa(const nk_eds_epa *sm);

struct eds_epa__attr
{
    const char *name;
    eds_epa *instance;
};

nk_eds_error
eds_epa__create(const struct eds_epa__attr *atrr, eds_epa **epa);

nk_eds_error
eds_epa__delete(eds_epa *epa);

nk_eds_error
eds_epa__add(eds_epa *epa, nk_eds_epa *sm);

nk_eds_error
eds_epa__remove(eds_epa *epa, nk_eds_epa *sm);

nk_eds_error
eds_epa__start_all(eds_epa *epa);

nk_eds_error
eds_epa__stop_all(eds_epa *epa);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_H_ */
