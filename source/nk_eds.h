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
typedef struct eds_object__sm eds_sm;
typedef struct eds_object__event eds_event;
typedef uint32_t eds_error;

#define EDS_ERROR__NONE                     0
#define EDS_ERROR__INVLD_ARGUMENT           0x01
#define EDS_ERROR__NO_MEMORY                0x02
#define EDS_ERROR__NO_RESOURCE              0x03
#define EDS_ERROR__EXISTS                   0x04
#define EDS_ERROR__INVLD_CONFIGURATION      0x05

#define EDS__ALLOCATOR__N_OF_INSTANCES      16u
#define EDS__ALLOCATOR__MIN_BYTES           16u

#define EDS__SM__DEFAULT_QUEUE_N_OF_ENTRIES 16u
#define EDS__SM__DEFAULT_PRIO               16u
#define EDS__SM__DEFAULT_NAME               "nameless"

/**
 * @brief       Add memory allocator for EDS objects creation.
 *
 * This functions needs to be called before any other function from EDS package. The function will
 * store the information about available allocators. Allocators are used by create functions of EDS
 * when creationg new objects. All objects used by EDS may be allocated dynamically or statically.
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
 *              previosly allocated block and it should recycle its space. This function does not
 *              need to handle NULL deallocations.
 * @param       context is pointer to allocator context structure if it needed by allocator. The
 *              structure needs to exist the whole time while allocator functions are being used.
 * @param       max_size is maximum size of block that can be allocated and freed by allocator. In
 *              case when a pool memory is used this argument will be equal to pool  memory block
 *              size. When this argument is set to zero or SIZE_MAX value then this allocator would
 *              be used in case no other allocator sattisifes required size.
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
eds_error
eds__add_allocator(void* (*alloc)(void *, size_t),
                   void  (*dealloc)(void*, void*),
                   void *  context,
                   size_t  max_size);

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
eds_error
eds_event__create(uint32_t     event_id,
                  size_t       event_data_size,
                  eds_event ** event);

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
eds_error
eds_event__init(eds_event * event,
                uint32_t    event_id,
                size_t      event_data_size);

/**
 * @brief       Get event identification number from the event.
 *
 * @pre         Argument @a event must be a non-NULL pointer and pointing to a previosly initialized
 *              or created event.
 * @param       event Pointer to event.
 * @return      Event identification number (uint32_t).
 */
uint32_t
eds_event__id(const eds_event * event);

/**
 * @brief       Get attached data from the event.
 *
 * @pre         Argument @a event must be a non-NULL pointer and pointing to a previosly initialized
 *              or created event.
 * @param       event Pointer to event.
 * @return      Pointer to event data.
 * @retval      NULL The event has no attached data.
 */
void *
eds_event__data(eds_event * event);

/**
 * @brief       Get event attached data size from the event.
 *
 * @pre         Argument @a event must be a non-NULL pointer and pointing to a previosly initialized
 *              or created event.
 * @param       event Pointer to event.
 * @return      Size of event attached data in bytes.
 * @retval      0 The event has no attached data.
 */
size_t
eds_event__size(const eds_event * event);

struct eds_object__mem;

size_t
eds__p__event__calculate_bundle_size(size_t event_data_size);

struct eds_object__event *
eds__p__event__allocate(size_t                  event_data_size,
                        struct eds_object__mem *mem);

void
eds__p__event__deallocate(struct eds_object__event *event);

void
eds__p__event__init(struct eds_object__event *event,
                    uint32_t                  event_id,
                    size_t                    event_data_size,
                    struct eds_object__mem *  mem);

void
eds__p__event__ref_up(struct eds_object__event * event);

void
eds__p__event__ref_down(struct eds_object__event * event);

bool
eds__p__event__is_in_use(const struct eds_object__event * event);

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

/**
 * @brief       Create and initialize a state machine.
 *
 * @param       initial_state
 * @param       sm_workspace
 * @param       attr
 * @param [out] sm
 * @return eds_error
 */
eds_error
eds_sm__create(eds_sm__state_fn *         initial_state,
               void *                     sm_workspace,
               const struct eds_sm__attr *attr,
               eds_sm **                  sm);

eds_error
eds_sm__delete(eds_sm *sm);

eds_error
eds_sm__send_signal(eds_sm * sm,
                    uint32_t event_id,
                    uint32_t timeout_ms);

eds_error
eds_sm__send_event(eds_sm *         sm,
                   const eds_event *event,
                   uint32_t         timeout_ms);

eds_error
eds_sm__send_event_after(eds_sm *         sm,
                         const eds_event *event,
                         uint32_t         after_ms);

eds_error
eds_sm__send_event_every(eds_sm *         sm,
                         const eds_event *event,
                         uint32_t         every_ms);

eds_epa*
eds_sm__get_epa(const eds_sm *sm);

struct eds_epa__attr
{
    const char *name;
    eds_epa *instance;
};

eds_error
eds_epa__create(const struct eds_epa__attr *atrr, eds_epa **epa);

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
