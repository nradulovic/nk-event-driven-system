/**
 * @file
 * @date        Dec 2, 2021
 * @author      Nenad Radulovic (nenad.b.radulovic@gmail.com)
 *
 * @brief       Nano-Kit Event Driven System (EDS) objects header
 *
 * @defgroup    eds_object Objects
 * @brief       Event Driven System (EDS) objects
 *
 * This module contains definition of all objects used in EDS.
 *
 * @{
 */

#ifndef NEON_KIT_GENERIC_SOURCE_NK_EDS_OBJECT_H_
#define NEON_KIT_GENERIC_SOURCE_NK_EDS_OBJECT_H_

#include <stdint.h>
#include <stddef.h>

/* Include port specific type definitions */
#include "eds_port.h"

/* Include EDS configuration */
#include "eds_config.h"

/* Include NK EDS API integral types */
#include "eds.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief       Vector object
 * @note        All members of this structure are private.
 */
struct eds_object__vector
{
    uint32_t p__n_entries;                      //!< Current number of entries in vector.
    uint32_t p__n_size;                         //!< Maximum number of entries in vector.
    size_t p__item_size;                        //!< Size of an entry in bytes.
    void * p__entries;                           //!< Pointer to array which contains all entries.
};

/**
 * @brief       List node object.
 * @note        All members of this structure are private.
 */
struct eds_object__list
{
    struct eds_object__list * p__next;          //!< Next node in linked list.
    struct eds_object__list * p__prev;          //!< Previous node in linked list.
};

/**
 * @brief       Memory object.
 * @note        All members of this structure are private.
 */
struct eds_object__mem
{
    /**
     * @brief       Allocator method
     *
     * Allocator method receives two arguments:
     * - pointer to a context (specified by @ref p__context member)
     * - size of memory block which needs to be allocated.
     *
     * Returns pointer to allocated memory section. If NULL pointer is returned then the allocation
     * memory has been depleted.
     */
    void * (* p__alloc)(void *, size_t);

    /**
     * @brief       Deallocator method
     *
     * Deallocator method receives two arguments:
     * - pointer to a context (specified by @ref p__context member)
     * - pointer to a previously allocated memory.
     */
    void (* p__dealloc)(void *, void *);
    void * p__context;                          //!< Memory context
    size_t p__max_size;                         //!< Max size of allocated memory block in bytes.
};

/**
 * @brief       Queue object
 * @note        All members of this structure are private.
 */
struct eds_object__queue
{
    uint32_t p__head;                           //!< Index pointing to head of queue.
    uint32_t p__tail;                           //!< Index pointing to tail of queue.
    uint32_t p__n_entries;                      //!< How many entries are put into the queue.
    uint32_t p__n_free;                         //!< How many entries are still free.
    void ** p__storage;                         //!< Pointer to the array which contains entries.
};

/**
 * @brief       Event queue object
 */
struct eds_object__equeue
{
    struct eds_object__queue queue;             //!< Queue instance.
};

/**
 * @brief       Timer sentinel state enumerator. 
 */
enum eds_object__tmr_sentinel_state
{
    EDS_OBJECT__TMR_SENTINEL_STATE_DORMANT,
    EDS_OBJECT__TMR_SENTINEL_STATE_INITIALIZED,
    EDS_OBJECT__TMR_SENTINEL_STATE_RUNNING
};

/**
 * @brief       Timer sentinel object.
 */
struct eds_object__tmr_sentinel
{
    struct eds_object__list p__active;          //!< List of active timers.
    struct eds_object__list p__pending;         //!< List of timers which wait for being activated.
    enum eds_object__tmr_sentinel_state state;
};

/**
 * @brief       Timer node state enumerator.
 */
enum eds_object__tmr_state
{
    EDS_OBJECT__TMR_STATE_DORMANT,              //!< Timer is in dormant state (not running).
    EDS_OBJECT__TMR_STATE_PENDING,              //!< Timer is pending to be activated.
    EDS_OBJECT__TMR_STATE_ACTIVE                //!< Timer is currently active and running.
};

/**
 * @brief       Timer node object
 * @note        All members of this structure are private.
 */
struct eds_object__tmr
{
    struct eds_object__list p__list;            //!< Linked list node.
    uint32_t rtime_ticks;                       //!< Relative time in ticks.
    uint32_t itime_ticks;                       //!< Initial time in ticks.
    void (* fn)(struct eds_object__tmr *);      //!< Callback function.
    enum eds_object__tmr_state state;           //!< State of timer node.
};

/**
 * @brief       Tasker object.
 * @note        All members of this structure are private.
 */
struct eds_object__tasker
{
    uint32_t p__pending_group;                  //!< Priority bit-field groups of pending tasks.

    /**
     * @brief       Priority groups of pending tasks.
     */
    struct eds_object__list p__pending_groups[32];
};

/**
 * @brief       Tasker node object.
 * @note        All members of this structure are private.
 */
struct eds_object__tasker_node
{
    struct eds_object__list p__list;            //!< Linked list node.
    uint_fast8_t p__prio;                       //!< Priority of task node.
};

/**
 * @brief       Event object.
 * @note        All members of this structure are private.
 */
struct eds_object__evt
{
    uint32_t p__id;                             //!< Unique ID number of event.
    uint32_t p__ref_count;                      //!< Event reference counter.
    size_t p__size;                             //!< Total size of an event in bytes.
    struct eds_object__mem * p__mem;             //!< Memory allocator reference.
};

/**
 * @brief       Event timer node object
 * @note        All members of this structure are private.
 */
struct eds_object__etm
{
    struct eds_object__tmr p__node;             //!< Timer instance.
    struct eds_object__evt p__evt;              //!< Event associated with this timer.
    struct eds_object__epa * p__epa;            //!< Owner of this timer.
    struct eds_object__mem * p__mem;            //!< Memory allocator reference.
};

/**
 * @brief       State machine processor action type
 */
typedef eds__sm_action eds_object__smp_action;

/**
 * @brief       State machine processor state function type
 */
typedef eds__sm_state eds_object__smp_state;

/**
 * @brief       State Machine Processor (SMP) object.
 * @note        All members of this structure are private.
 */
struct eds_object__smp
{
    eds_object__smp_state * p__state;            //!< Current state of state machine.
    void * p__workspace;                         //!< State machine workspace.
};

/**
 * @brief       Event Processing Agent (EPA) object.
 * @note        All members of this structure are private.
 */
struct eds_object__epa
{
    struct eds_object__tasker_node p__task;     //!< Task instance which is processing events.
    struct eds_object__smp p__smp;              //!< State machine processor instances.
    struct eds_object__equeue p__equeue;        //!< Event queue instance.
    struct eds_object__epn * p__epn;            //!< Owner of this EPA.
    struct eds_object__mem * p__mem;            //!< Memory allocator reference.
#if (EDS_CONFIG__AGENT__ENABLE_NAME != 0)
    const char * p__name;                       //!< Pointer to C string containing the name of EPA.
#endif
};

/**
 * @brief       Event Processing Network (EPN) object.
 * @note        All members of this structure are private.
 */
struct eds_object__epn
{
    struct eds_object__list p__list;            //!< Linked list node.
    struct eds_object__tasker p__tasker;        //!< Tasker which processes the tasks.
#if (EDS_PORT__GLOBAL_SLEEP == 0)
    struct eds_port__sleep p__sleep;            //!< Portable sleep data.
#endif
    bool p__should_run;                         //!< Stop execution flag (used to terminate EPN).
    struct eds_object__mem * p__mem;            //!< Memory allocator reference.
    const struct eds__network_attr * p__attr;   //!< Attributes of the network.
};

#ifdef __cplusplus
}
#endif

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_OBJECT_H_ */
/** @} */
