/**
 * @file
 * @date        Dec 2, 2021
 * @brief       Nano-Kit Event Driven System (EDS) interface header
 *
 * Main API header file. This is usually the only needed header file for application to use the
 * Event Driven System. It does not include additional header files besides standard C files. All types
 * defined in this header are opaque types to objects. The application uses
 * `eds__<object>_create` functions to allocate objects.
 *
 * This header defines:
 * - Public API macros
 * - Public API integral types
 * - Public API opaque types
 * - Public API functions
 *
 * @author      Nenad Radulovic (nenad.b.radulovic@gmail.com)
 * @authors     Nenad Radulovic (nenad.b.radulovic@gmail.com)
 *
 * @defgroup    eds_intf Interface
 * @brief       Event Driven System (EDS) interface
 *
 * @{
 */

#ifndef NEON_KIT_GENERIC_SOURCE_NK_EDS_H_
#define NEON_KIT_GENERIC_SOURCE_NK_EDS_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @defgroup    eds_system EDS management
 * @brief       EDS management interface.
 * @{
 */

/**
 * @defgroup    eds_objects Objects
 * @brief       Objects declarations.
 * @{
 */

/**
 * @brief       EDS memory (MEM) object type.
 */
typedef struct eds_object__mem eds__mem;

/**
 * @brief       EDS event (EVT) object type.
 */
typedef struct eds_object__evt eds__event;

/**
 * @brief       EDS state machine (SM) object type.
 */
typedef struct eds_object__smp eds__sm;

/**
 * @brief       EDS event processing agent (EPA) object type.
 */
typedef struct eds_object__epa eds__agent;

/**
 * @brief       EDS event timer (ETM) object type.
 */
typedef struct eds_object__etm eds__etimer;

/**
 * @brief       EDS event processing channel (EPC) object type.
 */
typedef struct eds_object__epc eds__channel;

/**
 * @brief       EDS event processing network (EPN) object type.
 */
typedef struct eds_object__epn eds__network;

/** @} */
/**
 * @defgroup    eds_defaults Defaults
 * @brief       Defaults interface.
 * @{
 */

/**
 * @brief       Default maximum memory entries that can be handled by EDS.
 *
 * This setting cannot be changed by global configuration (@ref eds_config).
 *
 * @see         eds_mem
 */
#define EDS__DEFAULT_MEM_ENTRIES            8u

/**
 * @brief       Default minimum bytes of a memory pool allocator.
 *
 * No memory pool allocator can be added if its allocation sizes are smaller than this macro.
 *
 * This setting cannot be changed by global configuration (@ref eds_config).
 *
 * @see         eds_mem
 */
#define EDS__DEFAULT_MEM_MIN_BYTES          16u

/**
 * @brief       Default number of events in event queue of event processing agent.
 *
 * This setting cannot be changed by global configuration (@ref eds_config), but for each created
 * event processing agent you can tweak this setting by supplying the attribute structure
 * (@ref eds__agent_attr) to @ref eds__agent_create function.
 *
 * @see         eds_agent
 */
#define EDS__DEFAULT_EPA_QUEUE_ENTRIES      16u

/**
 * @brief       Default event processing agent priority.
 *
 * This setting cannot be changed by global configuration (@ref eds_config), but for each created
 * event processing agent you can tweak this setting by supplying the attribute structure
 * (@ref eds__agent_attr) to @ref eds__agent_create function.
 *
 * @see         eds_agent
 */
#define EDS__DEFAULT_EPA_PRIO               16u

/**
 * @brief       Default event processing agent name.
 *
 * This setting cannot be changed by global configuration (@ref eds_config), but for each created
 * event processing agent you can tweak this setting by supplying the attribute structure
 * (@ref eds__agent_attr) to @ref eds__agent_create function.
 *
 * @see         eds_agent
 */
#define EDS__DEFAULT_EPA_NAME               "nameless"

/**
 * @brief       Default event processing network name.
 *
 * This setting cannot be changed by global configuration (@ref eds_config), but for each created
 * event processing agent you can tweak this setting by supplying the attribute structure
 * (@ref eds__agent_attr) to @ref eds__agent_create function.
 *
 * @see         eds_network
 */
#define EDS__DEFAULT_NETWORK_NAME           "nameless"

/**
 * @brief       Event timer rounding strategy
 *
 * When a timer is started and a time given as argument in milliseconds, it is converted from
 * milliseconds to number of ticks. By default, the given time is converted to ticks using
 * @ref EDS__ETIMER_FLAG__UP_TO strategy.
 *
 * This setting cannot be changed by global configuration (@ref eds_config), but for each created
 * event timer instance you can tweak this setting by supplying the attribute structure
 * (@ref eds__etimer_attr) to @ref eds__etimer_create function.
 */
#define EDS__DEFAULT_ETIMER_ROUND_STRATEGY  EDS__ETIMER_FLAG__UP_TO

/** @} */
/**
 * @defgroup    eds_version Version information
 * @brief       Version information interface.
 * @{
 */

/**
 * @brief       Version identification macro
 *
 * This macro definition contains current EDS version triplet: major, minor and patch numbers.
 * * The major number is located on bit positions [23 - 16].
 * * The minor number is located on bit positions [15 - 8].
 * * The patch number is located on bit positions [7 - 0].
 */
#define EDS__VERSION                        0x030005

/** @} */
/**
 * @defgroup    eds_errors Error handling
 * @brief       Error handling interface.
 * @{
 */

/**
 * @brief       EDS error type.
 */
typedef uint_fast8_t eds__error;

/**
 * @brief       No error has occurred.
 */
#define EDS__ERROR_NONE                     0

/**
 * @brief       Invalid argument is supplied to function.
 *
 * This usually means an argument pointer has a NULL value, but it might mean that pointed object
 * has failed some internal cheks.
 */
#define EDS__ERROR_INVALID_ARGUMENT         0x01

/**
 * @brief       No available memory.
 *
 * This error happens during memory allocation of an object and when memory allocator returns NULL
 * pointer.
 */
#define EDS__ERROR_NO_MEMORY                0x02

/**
 * @brief       No resource is available.
 */
#define EDS__ERROR_NO_RESOURCE              0x03

/**
 * @brief       No permission to execute an action on the object.
 */
#define EDS__ERROR_NO_PERMISSION            0x04

/**
 * @brief       Some resource or object already exists.
 */
#define EDS__ERROR_ALREADY_EXISTS           0x05

/**
 * @brief       Invalid configuration was specified in attribute structure.
 */
#define EDS__ERROR_INVALID_CONFIGURATION    0x06

/**
 * @brief       Referenced or requested object does not exist.
 */
#define EDS__ERROR_NOT_EXISTS               0x07

/**
 * @brief       Malformed state machine.
 */
#define EDS__ERROR_MALFORMED_SM             0x08

/**
 * @brief       There is no enough memory space to operate on.
 */
#define EDS__ERROR_NO_SPACE                 0x09

/**
 * @brief       Argument or attribute is out of range.
 */
#define EDS__ERROR_OUT_OF_RANGE             0x0a

/**
 * @brief       State machine has bad enter sequence.
 */
#define EDS__ERROR_SM_BAD_ENTER             0x0b

/**
 * @brief       State machine has bad exit sequence.
 */
#define EDS__ERROR_SM_BAD_EXIT              0x0c

/**
 * @brief       State machine has bad init sequence.
 */
#define EDS__ERROR_SM_BAD_INIT              0x0d

/**
 * @brief       State machine has bad super sequence.
 */
#define EDS__ERROR_SM_BAD_SUPER             0x0e

/**
 * @brief       Convert EDS error to human readable string.
 *
 * @param       error is integer value of an EDS error. For possible values refer to @see eds_errors
 * @return      Pointer to constant string describing the error. This pointer can not be a NULL
 *              pointer. In case when error value is not recognized by the function, it will return
 *              string with text "unknown".
 */
const char*
eds__error_to_str(uint32_t error);

/** @} */

eds__error
eds__initialize(void);

/**
 * @defgroup    eds_mem Memory allocator
 * @brief       Memory allocator interface.
 *
 * Neon-Kit EDS does not support standard C use of heap memory allocation (using malloc and free)
 * since many embedded projects and system impose a lot of restrictions on memory allocation
 * methods. In order to support dynamic nature of events, Neon-Kit EDS still needs to have some
 * support for dynamic memory allocation. Since the memory allocation restrictions change from
 * application to application (or project to project) it was decided that memory allocation problem
 * is pushed to the application writer.
 *
 * The application writer best knows what are circumstances where the application is going to be
 * executed and he has the best view in order to decide on allocation algorithm. Whatever the
 * application writer decides to use (memory pools or heap) all it needs to do is to tell to
 * Neon-Kit which allocator to use, using @ref eds__mem_add_allocator function.
 *
 * @{
 */

/**
 * @brief       Allocator function type.
 *
 * This type defines pointer to function which receives the following arguments:
 * - pointer to a context (which was supplied to @ref eds__mem_add_allocator function)
 * - size of memory block which needs to be allocated.
 *
 * The method returns pointer to allocated memory section. If NULL pointer is returned then the
 * allocation memory has been depleted.
 */
typedef void*
(eds__mem_alloc_fn)(void*, size_t);

/**
 * @brief       Deallocator function type.
 *
 * Deallocator method receives two arguments:
 * - pointer to a context (which was supplied to @ref eds__mem_add_allocator function)
 * - pointer to a previously allocated memory by @ref eds__mem_alloc_fn function.
 */
typedef void
(eds__mem_dealloc_fn)(void*, void*);

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
 * EDS can accept up to @ref EDS__DEFAULT_MEM_ENTRIES different allocators. It was shown in
 * practice that around 7 pool allocators of different sizes are needed for a typical application.
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
 *              need to handle NULL pointer deallocations.
 * @param       context is pointer to allocator context structure if it needed by allocator. The
 *              structure needs to exist the whole time while allocator functions are being used.
 * @param       max_size is maximum size of block that can be allocated and freed by allocator. In
 *              case when a pool memory is used this argument will be equal to pool  memory block
 *              size. When this argument is set to zero or SIZE_MAX value then this allocator would
 *              be used in case no other allocator satisfies required size.
 *
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE (@ref EDS__ERROR_NONE) Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT (@ref EDS__ERROR_INVALID_ARGUMENT) Is returned when
 *              either @a alloc or @a dealloc arguments are NULL pointer or when @a max_size is less
 *              than @ref EDS__DEFAULT_MEM_MIN_BYTES bytes.
 * @retval      EDS__ERROR_NO_RESOURCE (@ref EDS__ERROR_NO_RESOURCE) Is returned when application
 *              tried to add more than @ref EDS__DEFAULT_MEM_ENTRIES instances of memory allocator.
 * @retval      EDS__ERROR_ALREADY_EXISTS (@ref EDS__ERROR_ALREADY_EXISTS) The allocator with
 *              @a max_size memory block was already added.
 * @retval      EDS__ERROR_NO_PERMISSION (@ref EDS__ERROR_NO_PERMISSION) The memory manager cannot
 *              accept this memory allocator because a `create` function was already called.
 */
eds__error
eds__mem_add_allocator(eds__mem_alloc_fn * alloc,
    eds__mem_dealloc_fn * dealloc,
    void * context,
    size_t max_size);

/** @} */
/**
 * @defgroup    eds_event Event (EVT)
 * @brief       Event interface.
 * @{
 */

/**
 * @defgroup    eds_event_predefined Predefined events
 * @brief       Predefined events identifiers
 *
 * @{
 */

/**
 * @brief       Invalidated event identifier
 */
#define EDS__EVENT__NULL                    0

/**
 * @brief       Separator between EDS predefined events and application (user) events
 */
#define EDS__EVENT__USER                    64

/** @} */
/**
 * @name        Event generation functions
 * @{
 */
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
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when either @a event_id is equal to zero or
 *              when @a event pointer is NULL pointer.
 * @retval      EDS__ERROR_NO_RESOURCE When no suitable allocator was found and EDS is then unable
 *              to allocate memory space for the event. To add allocators use
 *              @ref eds__add_allocator function.
 * @retval      EDS__ERROR_NO_MEMORY When a suitable allocator was found but its memory reserves
 *              have been depleted.
 */
eds__error
eds__event_create(uint32_t event_id, size_t event_data_size, eds__event ** event);

/**
 * @brief       Initialize a statically allocated event.
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
 * @retval      EDS__ERRROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVLD_ARGUMENT Is returned when either @a event_id is equal to zero or
 *              when @a event pointer is NULL pointer.
 */
eds__error
eds__event_init(eds__event * event, uint32_t event_id, size_t event_data_size);

/** @} */

/**
 * @name        Event write access functions
 * @{
 */
/**
 * @brief       Put data into event
 *
 * @pre         Argument @a event must be a non-NULL pointer and pointing to a previously initialized
 *              or created event.
 * @param       event Pointer to event.
 * @return      Memory allocated by event where data can be put. The returned pointer will be NULL
 *              pointer if created event has been specified to have 0 bytes data.
 */
void*
eds__event_put_data(eds__event * event);

/** @} */
/**
 * @name        Event read access functions
 * @{
 */
/**
 * @brief       Get event identification number from the event.
 *
 * @pre         Argument @a event must be a non-NULL pointer and pointing to a previously initialized
 *              or created event.
 * @param       event Pointer to event.
 * @return      Event identification number (uint32_t).
 */
uint32_t
eds__event_id(const eds__event * event);

/**
 * @brief       Get attached data from the event.
 *
 * @pre         Argument @a event must be a non-NULL pointer and pointing to a previously initialized
 *              or created event.
 * @param       event Pointer to event.
 * @return      Pointer to event data.
 * @retval      NULL The event has no attached data.
 */
const void*
eds__event_data(const eds__event * event);

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
eds__event_size(const eds__event * event);

/** @} */
/**
 * @name        Event management functions
 * @{
 */
/**
 * @brief       Cancel an event
 *
 * Once the event is sent to an Agent, the only way to undo this action is to cancel the event. The
 * event is cancelled to all receiving Agents to which the event has been sent.
 *
 * @param       event Pointer to event.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a event pointer is NULL pointer.
 * @retval      EDS__ERROR_NO_PERMISSION Is returned when event is statically allocated and that
 *              class of events can not be canceled.
 * @note        If event is statically allocated it can not be canceled.
 */
eds__error
eds__event_cancel(eds__event * event);

/**
 * @brief       Prevent event from being recycled (deleted) by the system
 *
 * Every event which is processed by all Agents that received it is then being deleted by EDS. After
 * call to this function, the event won't be recycled by the system. The event will remain valid
 * until @ref eds__event_toss is called.
 *
 * @param       event Pointer to event.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a event pointer is NULL pointer.
 * @retval      EDS__ERROR_NO_PERMISSION Is returned when event is statically allocated and that
 *              class of events can not be recycled and therefore protected from it. In case static
 *              event is passed, this function will be NOP (no-operation) function.
 */
eds__error
eds__event_keep(const eds__event * event);

/**
 * @brief       Allow event recycling
 *
 * When an event is processed by all agents that it was sent to, EDS will automatically recycle it
 * (delete). Use @ref eds__event_keep to prevent the event from being recycled and use this function
 * to allow again the event recycling.
 *
 * @param       event Pointer to event.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a event pointer is NULL pointer.
 * @retval      EDS__ERROR_NO_PERMISSION Is returned when event is statically allocated and that
 *              class of events can not be recycled and therefore protected from it. In case static
 *              event is passed, this function will be NOP (no-operation) function.
 */
eds__error
eds__event_toss(const eds__event * event);
/** @} */
/** @} */
/**
 * @defgroup    sm State machine (SM)
 * @brief       State machine interface
 * @{
 */

/**
 * @defgroup    eds_sm_events Predefined state machine events
 * @brief       Predefined state machine events
 * @{
 */

/**
 * @brief       Predefined event - initialize
 *
 * This event is generated by state machine processor (code which executes a state machine) when
 * state machine has reached its destination state during a state transition.
 */
#define EDS__SM_EVENT__INIT                 1

/**
 * @brief       Predefined event - enter
 *
 * This event is generated by state machine processor (code which executes a state machine) when
 * state machine is entering a state during a state transition.
 */
#define EDS__SM_EVENT__ENTER                2

/**
 * @brief       Predefined event - exit
 *
 * This event is generated by state machine processor (code which executes a state machine) when
 * state machine is exiting a state during a state transition.
 */
#define EDS__SM_EVENT__EXIT                 3

/**
 * @brief       Predefined event - super
 *
 * This event is generated by state machine processor (code which executes a state machine) when
 * state machine processor wants to know what is the super state of the current state.
 */
#define EDS__SM_EVENT__SUPER                4

/** @} */

/**
 * @defgroup    eds_sm_state State definition
 * @brief       State definition interface
 * @{
 */

/**
 * @brief       Return value type of state function.
 *
 * State function is using its return value to tell to state machine processor what action to take
 * further.
 */
typedef uint_fast8_t eds__sm_action;

/**
 * @brief       State function type
 *
 * Each state receives the pointers to state machine, state machine workspace and pointer to event
 * which needs to be processed. After processing the event, the state function tells to state
 * machine processor what further action to take by returning a value.
 */
typedef eds__sm_action
(eds__sm_state)(eds__sm * sm, void * workspace, const eds__event * event);

/** @} */
/**
 * @defgroup    eds_state_actions State return actions
 * @brief       State return actions
 * @{
 */

/**
 * @brief       Event was handled
 * @param       sm Pointer to state machine.
 * @return      Return a value to tell state machine processor that event was handled and no further
 *              action is required.
 */
eds__sm_action
eds__sm_event_handled(eds__sm * sm);

/**
 * @brief       Event was ignored
 * @param       sm Pointer to state machine.
 * @return      Return a value to tell state machine processor that event was ignored and no further
 *              action is required.
 */
eds__sm_action
eds__sm_event_ignored(eds__sm * sm);

/**
 * @brief       Event was handled causing the state machine to change its state
 * @param       sm Pointer to state machine.
 * @return      Return a value to tell state machine processor that a transition needs to be
 *              executed.
 */
eds__sm_action
eds__sm_transit_to(eds__sm * sm, eds__sm_state * new_state);

/**
 * @brief       Event was not handled causing the state to tell its super state to state machine
 *              processor
 * @param       sm Pointer to state machine.
 * @return      Return a value to tell state machine processor that a super state is being returned.
 */
eds__sm_action
eds__sm_super_state(eds__sm * sm, eds__sm_state * super_state);

/** @} *//**
 * @defgroup    eds_sm_states Predefined state machine states
 * @brief       Predefined state machine states
 * @{
 */

/**
 * @brief       State machine top state
 * @param       sm Pointer to state machine instance
 * @param       workspace Pointer to state machine workspace structure instance
 * @param       event Pointer to event
 * @return      After processing the event, the state function tells to state
 *              machine processor what further action to take by returning a value. For return
 *              values details see @ref eds_state_actions.
 */
eds__sm_action
eds__sm_top_state(eds__sm * sm, void * workspace, const eds__event * event);

/** @} */
/** @} */
/**
 * @defgroup    eds_agent Event Processing Agent (EPA)
 * @brief       Event Processing Agent interface
 * @{
 */

/**
 * @defgroup    eds_agent_attr Agent customization
 * @brief       Customization of agent instance using attributes
 *
 * Attributes describe in greater details an agent instance. When no attribute is being supplied to
 * @ref eds__agent_create then agent instances are created using default attribute values specified
 * in @ref eds_defaults.
 * @{
 */

/**
 * @brief       Agent priority
 */
typedef enum eds_object__epa_prio
{
    EDS__EPA_PRIO__IDLE = 0,            ///< Reserved for Idle EPA.
    EDS__EPA_PRIO__LOW = 1,            ///< Priority: low
    EDS__EPA_PRIO__LOW_1 = 1 + 1,        ///< Priority: low + 1
    EDS__EPA_PRIO__LOW_2 = 1 + 2,        ///< Priority: low + 2
    EDS__EPA_PRIO__LOW_3 = 1 + 3,        ///< Priority: low + 3
    EDS__EPA_PRIO__LOW_4 = 1 + 4,        ///< Priority: low + 4
    EDS__EPA_PRIO__BELOW_NORMAL = 6,            ///< Priority: below normal
    EDS__EPA_PRIO__BELOW_NORMAL_1 = 6 + 1,        ///< Priority: below normal + 1
    EDS__EPA_PRIO__BELOW_NORMAL_2 = 6 + 2,        ///< Priority: below normal + 2
    EDS__EPA_PRIO__BELOW_NORMAL_3 = 6 + 3,        ///< Priority: below normal + 3
    EDS__EPA_PRIO__BELOW_NORMAL_4 = 6 + 4,        ///< Priority: below normal + 4
    EDS__EPA_PRIO__NORMAL = 11,           ///< Priority: normal
    EDS__EPA_PRIO__NORMAL_1 = 11 + 1,       ///< Priority: normal + 1
    EDS__EPA_PRIO__NORMAL_2 = 11 + 2,       ///< Priority: normal + 2
    EDS__EPA_PRIO__NORMAL_3 = 11 + 3,       ///< Priority: normal + 3
    EDS__EPA_PRIO__NORMAL_4 = 11 + 4,       ///< Priority: normal + 4
    EDS__EPA_PRIO__NORMAL_5 = 11 + 5,       ///< Priority: normal + 5
    EDS__EPA_PRIO__ABOVE_NORMAL = 17,           ///< Priority: above normal
    EDS__EPA_PRIO__ABOVE_NORMAL_1 = 17 + 1,       ///< Priority: above normal + 1
    EDS__EPA_PRIO__ABOVE_NORMAL_2 = 17 + 2,       ///< Priority: above normal + 2
    EDS__EPA_PRIO__ABOVE_NORMAL_3 = 17 + 3,       ///< Priority: above normal + 3
    EDS__EPA_PRIO__ABOVE_NORMAL_4 = 17 + 4,       ///< Priority: above normal + 4
    EDS__EPA_PRIO__HIGH = 22,           ///< Priority: high
    EDS__EPA_PRIO__HIGH_1 = 22 + 1,       ///< Priority: high + 1
    EDS__EPA_PRIO__HIGH_2 = 22 + 2,       ///< Priority: high + 2
    EDS__EPA_PRIO__HIGH_3 = 22 + 3,       ///< Priority: high + 3
    EDS__EPA_PRIO__HIGH_4 = 22 + 4,       ///< Priority: high + 4
    EDS__EPA_PRIO__REALTIME = 27,           ///< Priority: realtime
    EDS__EPA_PRIO__REALTIME_1 = 27 + 1,       ///< Priority: realtime + 1
    EDS__EPA_PRIO__REALTIME_2 = 27 + 2,       ///< Priority: realtime + 2
    EDS__EPA_PRIO__REALTIME_3 = 27 + 3,       ///< Priority: realtime + 3
    EDS__EPA_PRIO__REALTIME_4 = 27 + 4,       ///< Priority: realtime + 4
} eds__epa_prio;

/**
 * @brief       Agent attribute structure.
 *
 * Use this structure to override and customize an agent instance to application needs.
 *
 * @note        When instancing this structure, all unused fields must be initialized to zero.
 */
struct eds__agent_attr
{
    /**
     * @brief   This member defines agent human readable name string.
     *
     * When this pointer is set to null, the newly created agent will have name as defined by
     * @ref EDS__DEFAULT_EPA_NAME. This string must exists the whole time the agent is running since
     * only reference to a string is stored in the attribute structure and in agent structure.
     */
    const char * name;

    /**
     * @brief   This member defines agent priority.
     *
     * When this member is set to zero, the newly created agent will have priority as defined by
     * @ref EDS__DEFAULT_EPA_PRIO.
     */
    eds__epa_prio prio;

    /**
     * @brief   This member defines maximum number of entries in event queue.
     *
     * When this member is set to zero, the newly created agent will have number of entries as
     * defined by @ref EDS__DEFAULT_EPA_QUEUE_ENTRIES.
     */
    uint32_t equeue_entries;

    /**
     * @brief   This member defines whether the agent instance should be allocated statically.
     *
     * In some application there is a requirement to allocate structures statically. If that is the
     * case use `static_` members of attribute class to allocate all parts of the agent instance
     * statically.
     *
     * To allocate the agent statically, both `static_instance` and `static_equeue_storage` needs
     * to be defined.
     */
    eds__agent * static_instance;

    /**
     * @brief   This member defines whether the agent event queue should be allocated statically.
     *
     * In some application there is a requirement to allocate structures statically. If that is the
     * case use `static_` members of attribute class to allocate all parts of the agent instance
     * statically.
     *
     * To allocate the agent statically, both `static_instance` and `static_equeue_storage` needs
     * to be defined.
     */
    eds__event ** static_equeue_storage;
};

/** @} */
/**
 * @brief       Create and initialize an agent (Event Processing Agent).
 *
 * @param[in]   sm_initial_state Pointer to initial state function of state machine.
 * @param[in]   sm_workspace Pointer to an allocated memory space reserved for state machine
 *              operation. Use this argument to pass pointer to structure instances which then can
 *              be used by state machine code.
 * @param[in]   attr Use @a attr pointer to pass attribute structure. Use attribute structure to
 *              customize agent instance. Pass NULL pointer to use defaults specified in
 *              @ref eds_defaults.
 * @param[out]  agent Pointer to pointer to agent. The pointer to agent will be filled after
 *              successful creation of the agent.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a sm_initial_state or @a agent pointer
 *              is NULL pointer.
 * @retval      EDS__ERROR_INVALID_CONFIGURATION Is returned when:
 *              1. The member `equeue_entries` in @a attr attribute structure is zero.
 *              2. Not both members `static_instance` and `static_equeue_storage` are set to
 *              non-NULL value. See the valid case examples in @ref eds__agent_attr description.
 * @retval      EDS__ERROR_NO_RESOURCE When no suitable allocator was found and EDS is then unable
 *              to allocate memory space for the agent. To add allocators use
 *              @ref eds__add_allocator function.
 * @retval      EDS__ERROR_NO_MEMORY When a suitable allocator was found but its memory reserves
 *              have been depleted.
 */
eds__error
eds__agent_create(eds__sm_state * sm_initial_state,
    void * sm_workspace,
    const struct eds__agent_attr * attr,
    eds__agent ** agent);

/**
 * @brief       Delete the agent.
 * @param[in]   agent Pointer to agent instance.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a agent pointer is NULL pointer.
 * @retval      EDS__ERROR_NO_PERMISSION When the agent is still designated to a network. Before an
 *              agent can be deleted it must be previously removed from the network by a call to
 *              @ref eds__network_remove_agent function.
 */
eds__error
eds__agent_delete(eds__agent * agent);

/**
 * @brief       Send the event to the agent.
 * @param[in]   agent Pointer to agent instance.
 * @param[in]   event Pointer to event instance which will be sent to the @a agent.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a agent or @a event pointer is NULL
 *              pointer.
 * @retval      EDS__ERROR_NO_PERMISSION When the agent is not designated to any network. In order
 *              to send an event to an agent, the agent must be previosly a part of a network.
 *              Insert the agent with a call to @ref eds__network_add_agent function.
 */
eds__error
eds__agent_send(eds__agent * agent, const eds__event * event);

/**
 * @brief       Get agent instance from state machine instance.
 * @param[in]   sm Pointer to state machine instance.
 * @return      Pointer to agent instance containing the state machine.
 * @pre         Argument @a sm must be a non-NULL pointer and pointing to a previously initialized
 *              instance of state machine.
 */
eds__agent*
eds__agent_from_sm(eds__sm * sm);

/**
 * @brief       Get the network this agent belongs to
 *
 * @param       agent Pointer to agent
 * @return      Pointer to network this agent belongs to.
 * @retval      NULL this agent does not belong to any network (undesignated agent) or @a agent
 *              pointer is NULL pointer.
 * @pre         Argument @a agent must be a non-NULL pointer and pointing to a previously
 *              initialized agent instance.
 */
eds__network*
eds__agent_network(const eds__agent * agent);

/**
 * @brief       Get the workspace of this agent
 *
 * @param       agent Pointer to agent
 * @return      void* Pointer to agent workspace which was passed to @ref eds__agent_create function
 *              during the creation of the agent.
 */
void*
eds__agent_workspace(const eds__agent * agent);

/** @} *//**
 * @defgroup    eds_etimer Event Timer (ETM)
 * @brief       Event Timer interface
 * @{
 */

/**
 * @defgroup    eds_etimer_attr Event timer customization
 * @brief       Customization of event timer instance using attributes
 *
 * Attributes describe in greater details an event timer instance. When no attribute is being
 * supplied to @ref eds__etimer_create then event timer instances are created using default
 * attribute values specified in @ref eds_defaults.
 * @{
 */
/**
 * @name        Event timers customization flags.
 * @{
 */

/**
 * @brief       Round up time up to specified time
 */
#define EDS__ETIMER_FLAG__UP_TO             0x1

/**
 * @brief       Set time at least to specified time
 */
#define EDS__ETIMER_FLAG__AT_LEAST          0x2

/** @} */

/**
 * @brief       Event timer attribute structure
 */
struct eds__etimer_attr
{
    /**
     * @brief   This member defines whether the event timer instance should be allocated statically.
     *
     * In some application there is a requirement to allocate structures statically. If that is the
     * case use `static_` members of attribute class to allocate all parts of the agent instance
     * statically.
     *
     * To allocate the event timer statically set this pointer to manually, statically allocated
     * instance.
     */
    eds__etimer * static_instance;

    /**
     * @brief   Attribute flags
     *
     * These flags define behavior of the timer. For available flags refer to Event timers
     * customization attributes.
     */
    uint32_t flags;
};
/** @} */

/**
 * @brief       Create an event timer.
 * @param[in]   attr Use @a attr pointer to pass attribute structure. Use attribute structure to
 *              customize event timer instance. Pass NULL pointer to use defaults specified in
 *              @ref eds_defaults.
 * @param[out]  etimer Pointer to pointer to event timer. The pointer to event timer will be filled
 *              after successful creation of the event timer.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a etimer pointer is NULL pointer.
 * @retval      EDS__ERROR_NO_RESOURCE When no suitable allocator was found and EDS is then unable
 *              to allocate memory space for the agent. To add allocators use
 *              @ref eds__add_allocator function.
 * @retval      EDS__ERROR_NO_MEMORY When a suitable allocator was found but its memory reserves
 *              have been depleted.
 */
eds__error
eds__etimer_create(const struct eds__etimer_attr * attr, eds__etimer ** etimer);

/**
 * @brief       Delete an event timer.
 * @param[in]   etimer Pointer to previously created event timer instance.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a etimer pointer is NULL pointer.
 */
eds__error
eds__etimer_delete(eds__etimer * etimer);

/**
 * @brief       Send @a event to @a agent using @a etimer after @a after_ms milliseconds.
 * @param[in]   etimer Pointer to previously initialized event timer instance which will be used
 *              for time keeping.
 * @param[in]   agent Pointer to previously initialized and designated agent instance which will
 *              receive the @a event.
 * @param[in]   event Pointer to event which is to be sent to the @a agent. Event timers support only
 *              dynamic events (the ones created by a call to @ref eds__event_create function).
 * @param       after_ms After this many milliseconds send @a event to @a agent.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a etimer, @a agent or @a event pointer
 *              is NULL pointer.
 * @retval      EDS__ERROR_NO_PERMISSION When the agent is not designated to any network. In order
 *              to send an event to an agent, the agent must be previously a part of a network.
 *              Insert the agent with a call to @ref eds__network_add_agent function.
 * @retval      EDS__ERROR_ALREADY_EXISTS When the event timer is already running. The timer needs
 *              to be canceled first by calling @ref eds__etimer_cancel before trying to re-use it.
 * @retval      EDS__ERROR_NO_RESOURCE When the given @a event is not a dynamic event.
 */
eds__error
eds__etimer_send_after(eds__etimer * etimer,
    eds__agent * agent,
    const eds__event * event,
    uint32_t after_ms);

/**
 * @brief       Send @a event to @a agent using @a etimer every @a after_ms milliseconds.
 * @param[in]   etimer Pointer to previously initialized event timer instance which will be used
 *              for time keeping.
 * @param[in]   agent Pointer to previously initialized and designated agent instance which will
 *              receive the @a event.
 * @param[in]   event Pointer to event which is to be sent to the @a agent. Event timers support only
 *              dynamic events (the ones created by a call to @ref eds__event_create function).
 * @param       after_ms After this many milliseconds send @a event to @a agent.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a etimer, @a agent or @a event pointer
 *              is NULL pointer.
 * @retval      EDS__ERROR_NO_PERMISSION When the agent is not designated to any network. In order
 *              to send an event to an agent, the agent must be previously a part of a network.
 *              Insert the agent with a call to @ref eds__network_add_agent function.
 * @retval      EDS__ERROR_ALREADY_EXISTS When the event timer is already running. The timer needs
 *              to be canceled first by calling @ref eds__etimer_cancel before trying to re-use it.
 * @retval      EDS__ERROR_NO_RESOURCE When the given @a event is not a dynamic event.
 */
eds__error
eds__etimer_send_every(eds__etimer * etimer,
    eds__agent * agent,
    const eds__event * event,
    uint32_t every_ms);

/**
 * @brief       Cancel (stop) a running timer.
 * @param[in]   etimer Pointer to previously initialized event timer instance which needs to be
 *              canceled if it is running.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVALID_ARGUMENT Is returned when @a etimer is NULL pointer.
 * @retval      EDS__ERROR_NOT_EXISTS When the event timer is not running. The timer needs to be
 *              started first by calling @ref eds__etimer_send_after or @ref eds__etimer_send_evety
 *              before trying to cancel it.
 */
eds__error
eds__etimer_cancel(eds__etimer * etimer);

/** @} */
/**
 * @defgroup    eds_network Event Processing Network (EPN)
 * @brief       Event Processing Network interface
 *
 * Event Processing Network is responsible for executing the agents when an agent receives an event.
 *
 * At least one network needs to exist in order to process the events.
 *
 * Agents which are part of a network are executing in the same thread context.
 *
 * Multiple networks might exist. Each network is executing agents in different thread context. Event
 * Driven System supports sending events between two networks.
 * @{
 */

/**
 * @defgroup    eds_network_attr Network customization
 * @brief       Customization of network instance using attributes
 *
 * Attributes describe in greater details a network instance. When no attribute is being supplied to
 * @ref eds__network_create then network instances are created using default attribute values
 * specified in @ref eds_defaults.
 * @{
 */

/**
 * @brief       Network attribute structure
 *
 * Attribute structure describes in greater details a network. When no attribute is being used,
 * networks are created using default values specified in @ref eds_defaults. Use this structure to
 * override and customize a network instance.
 */
struct eds__network_attr
{
    /**
     * @brief   This member defines network human readable name string.
     *
     * When this pointer is set to null, the newly created agent will have name as defined by
     * @ref EDS__DEFAULT_NETWORK_NAME. This string must exists the whole time the network is running
     * since only the reference to a string is stored in the attribute structure and in network
     * structure.
     */
    const char * name;

    /**
     * @brief   This member defines whether the network instance will be allocated statically.
     *
     * In some application there is a requirement to allocate structures statically. If that is the
     * case use `static_` members of attribute class to allocate all parts of the agent instance
     * statically.
     *
     * To allocate the network statically set this pointer to manually, statically allocated
     * instance.
     */
    eds__network * instance;

    /**
     * @brief   This callback is called when network wants to switch from __run__ state to __idle__ state.
     *
     * When this member is initialized to NULL then no callback is called.
     *
     * The callback receives:
     * - the pointer to current network instance and
     * - the global callback argument which is also part of this structure.
     *
     * @note    In order to use callbacks @ref EDS_CONFIG__NETWORK__ENABLE_CALLBACKS macro must be
     *          enabled.
     */
    void (* cb_to_idle)(eds__network *, void *);

    /**
     * @brief  This callback is called when network wants to switch from __idle__ state to __run__ state.
     *
     * When this member is initialized to NULL then no callback is called.
     *
     * The callback receives:
     * - the pointer to current network instance and
     * - the callback argument which is also part of this structure.
     *
     * @note    In order to use callbacks @ref EDS_CONFIG__NETWORK__ENABLE_CALLBACKS macro must be
     *          enabled.
     */
    void (* cb_to_run)(eds__network *, void *);

    /**
     * @brief  Callback argument
     *
     * Use this member to supply a some application related data to callbacks. This argument is just
     * passed to callbacks when they are called.
     *
     * @note    In order to use callbacks @ref EDS_CONFIG__NETWORK__ENABLE_CALLBACKS macro must be
     *          enabled.
     */
    void * cb_arg;
};

/** @} */
/**
 * @brief       Create a network.
 */
eds__error
eds__network_create(const struct eds__network_attr * atrr, eds__network ** network);

/**
 * @brief       Delete a network.
 */
eds__error
eds__network_delete(eds__network * network);

/**
 * @brief       Add an agent to network.
 */
eds__error
eds__network_add_agent(eds__network * network, eds__agent * sm);

/**
 * @brief       Remove an agent from the network.
 *
 * @param       network Pointer to event processing network.
 * @param       agent Pointer to agent instance which is to be removed from the network.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVLD_ARGUMENT Is returned when @a network or @a agent pointer is NULL
 *              pointer.
 * @retval      EDS__ERROR_NOT_EXISTS when the agent pointed by @a agent is not belonging to any
 *              network.
 */
eds__error
eds__network_remove_agent(eds__network * network, eds__agent * agent);

/**
 * @brief       Start executing Agents which belong to this network.
 */
eds__error
eds__network_start(eds__network * network);

/**
 * @brief       Stop executing Agents of this network.
 */
eds__error
eds__network_stop(eds__network * network);

/**
 * @brief       Get the name of the network
 *
 * @param       network Pointer to event processing network.
 * @param       name Pointer to `const char *` variable which will hold the network name.
 * @return      Operation status.
 * @retval      EDS__ERROR_NONE Operation completed successfully.
 * @retval      EDS__ERROR_INVLD_ARGUMENT Is returned when @a network or @a name pointer is NULL
 *              pointer.
 */
eds__error
eds__network_name(eds__network * network, const char ** name);

/** @} */
#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_H_ */
/** @} */
