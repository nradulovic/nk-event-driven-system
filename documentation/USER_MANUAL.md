# Abbreviations

The following is the table containing abbreviations used in the API and source code:
  
  | Word                       | Abbreviation |
  | -------------------------- | ------------ |
  | interrupt                  | int          |
  | interrupt service routine  | isr          |
  | interrupt request          | irq          |
  | priority                   | prio         |
  | configuration              | config       |
  | invalid                    | invld        |
  | illegal                    | ilgl         |
  | attribute                  | attr         |
# Application development

This document describes a typical application development cycle and how are Nano Kernel options 
configured for each development cycle.

# Development cycles

Typical software development start with:
1. __Analysis cycle__ - The requirement collection is the first stage in the application development 
   process. It is performed by the a team of business analysts, technical architects and project 
   managers together with all the stakeholders. It is also very beneficial to the project to include 
   domain experts in the industry during these initial steps.
2. __Project Design__ - Based on the requirements collection the team prepares the system and 
   software design to define the whole system architecture. This design phase will be a base for the 
   next coding phase. At this moment some ruff estimations can be made for resource usage (such as 
   number of threads, timers, pools, etc).
3. __Implementation__ - This is the longest phase of the software development life cycle. During 
   this phase more exact estimations are made on the needed resources. At this moment typical 
   application configuration has the following settings:
   - NK_CONFIG__ERROR_PRE_EN set to 1
   - NK_CONFIG__ERROR_POST_EN set to 1

4. __Testing__ - Once some part of the software implementation is complete, it is tested by the QA 
   team. During this phase typical applicaiton configuration also has the following set:
   - NK_CONFIG__PROFILING_EN set to 1
5. __Release__ - Once the software is developed and fully tested, the final deployments process is 
   launched. Typicall application configuration disables the following options:
   - NK_CONFIG__ERROR_PRE_EN set to 0
   - NK_CONFIG__ERROR_PROFILING_EN set to 0
   
   Some applications might even disable postcondition error handling:
   - NK_CONFIG__ERROR_POST_EN set to 0
6. __Maintainance__ - After the release, the application continues its cycle in the following ways:
    - bug fixing
    - software upgrade
    - enhancement with new features

# Application project architectures

## Single project application architecture

This is the most common project architecture. All the application and Nano Kernel sources are part
of one single project. Nano Kernel source is just a subfolder to application code base.

## Multiple projects applications architecture

It is possible to use and compile Nano Kernel sources as a library. The library is compiled as a
separate project which is only a part of the application. There are some limitations to this 
approach and the limitation dependend on the way the Nano Kernel sources are used:
- application code uses only `nk.h` header file
- application code uses other Nano Kernel headers as well 

### Application code uses only nk.h header file

In this configuration none of the Nano Kernel configuration details are exported outside the public
API. This is the cleanest way of using the Nano Kernel sources and it allows for easy compilation of
Nano Kernel sources as a library.

![Application development with public API only](images/app_dev_multi_no_types.png)

### Application code uses other Nano Kernel headers as well

In this configuration the application also uses protected Nano Kernel headers which are including 
the Nano Kernel configuration, too. For this reason, a 3rd project, which contains the 
configuration, needs to be created.

![Application development with public API and protected headers](images/app_dev_multi.png)

# Application main function

The main() function is normal function. Once the execution reaches main() there is a recommended 
order to initialize the hardware and start the kernel. This is also reflected in the user code 
template files.

The application's main() should implement at least the following in the given order:

* Initialization and configuration of hardware including peripherals, memory, pins, clocks and the 
  interrupt system.
* Initialize the NK kernel using nk_kernel__init() function.
* Optionally, create at least one thread (for example app_main), which is used as a main thread 
  using nk_thread__create() function. This thread should take care of creating and starting objects, 
  once it is run by the scheduler. Alternatively, threads can be created in main() function
  directly.
* Start the NK kernel using nk_kernel__start() which also configures the system tick timer and 
  initializes NK specific interrupts. This function does not return in case of successful execution. 
  Therefore, any application code after nk_kernel__start() will not be executed.

__Note__:

* Before executing any function from NK the function nk_kernel__init() must be called.

# Error handling

## Intro

- Error indicates a serious problems that a reasonable application should not try to catch.
- Exception indicates conditions that a reasonable application might want to catch.

Errors are to be handled by assertion macros like `assert` from standard C library `assert.h`. These
macros block the execution (some implementations also restart the whole device). 

Exceptions are to be handled by conditions in code. Since we are using C, the most simplest way is 
to use plain conditions and handle them by marking that an error has happened, but the execution 
continues.

__When not to use errors:__ It is be strictly forbidden to assert external signal value(s) or 
any signal aspect (this include values provided by MCU peripheral, too). This is to avoid and 
minimise sensitivity of the firmware to external influences which could lead to blocking the 
firmware execution or restarting the device.

__When to use errors:__ Only internal values are allowed to be asserted. Examples are, but not 
restricted to, memory or a resource allocation, NULL pointer checks. 

__When to use exceptions:__ When a value which is to be checked is coming from a peripheral (like 
ADC value is out of range) or from external component on a data bus (timeout on data bus).

All Neon Kernel API functions are reporting exceptions as return values. The following classes of 
errors are defined:
- precondition errors (P)
- postcondition errors (O)
- safety errors (S)
- general errors (G)

It is up to application to decide if an erroneous state is to be treated as an error or an exception.

Nano Kernel internally uses errors which are handled by `assert.h` standard C library header. The
application must define a function which implements the assert macro. This macro should never 
return.

__NOTE__:
- All erroneous states in Nano Kernel API functions are called errors. It is up to application to
  promote errors to exceptions as it sees fit.
- In the code and in the following text, both error and exception, are refered to as an error.
  
## Error reporting

Nano Kernel documentation defines the following terms:

- __originator error reporting__ - is a piece of code which is used by a function to report its 
  erronous state to someone. There are 3 possible methods for a function to report an erronous 
  state: 
  1. _return value_ - a function can return an error code to its caller. This is most common method 
     for reporting the error. The function itself is responsible for any cleanup if an error comes 
     up during the execution steps. This method is used by many open source projects including the 
     most prominent one, the Linux kernel.
  2. _global state_ - a function sets a global variable directly or by a call to another function 
     which stores the error information in some global state (variable). This method is used by some 
     embedded systems and by standard C library (for example, see errno). This method has drawback 
     when the system switched to multiple threads, then global state needs to be protected or 
     allocated on thread local storage (TLS).
  3. _bundle status flags_ - a function returns data values and data status flags bundled together. 
     All data types in a system are structures which contain data payload and data status flags. 
     When such a structure is returned by a callee function, the caller can see if data is valid or 
     not and what is the reason of failure. This kind of method is typically used in proprietary 
     SCADA systems, where all data is tagged with status flags and is passed between components as 
     such.
- __device exception reporting__ - defines how errors reported to external world some communication 
link or, very commonly, with LEDs. This piece of code handles the error values it got from various 
internal components and how to present that information to external world.

__NOTE__:
- Nano Kernel uses _return value_ method for originator error reporting.
 
## Error code bitfields

All error code use the bitfields to distinguish error classes.

Error code bitfields:

| Component  | P field | O field | S field | G field |
| ---------- | ------- | ------- | ------- | ------- |
| 4 bits     | 8 bits  | 6 bits  | 6 bits  | 8 bits  |
| 31 - 28    | 27 - 20 | 19 - 14 | 13 - 8  | 7 - 0   |

Where:
- __Component__ - specifies the origin of error. The origin of the error is specifined in terms of
  components.
- __P field__ - specifies if the error is a precondition error (P). If this bitfield is zero then
  the error is not a __P__ class of error.
- __O field__ - specifies if the error is a postcondifion error (O). If this bitfield is zero then
  the error is not a __O__ class of error.
- __S field__ - specifies if the error is a safety error (S). If this bitfield is zero then the
  error is not a __S__ class of error.
  
No error can not have all POSG fields set to zero. One exception to this is NK_ERROR__NONE where all
fields are set to zero.
    
## Error mapping

### General error - specific error mapping
    
| General error                       | Specific error                      |
| ----------------------------------- | ----------------------------------- |
| NK_ERROR__INVLD_ARGUMENT            | NK_ERROR__POOL_P_INVLD_POOL         |
| ^                                   | NK_ERROR__POOL_P_INVLD_BLOCK        |
| ^                                   | NK_ERROR__POOL_P_INVLD_N_BLOCKS     |
| ^                                   | NK_ERROR__POOL_P_INVLD_BLOCK_SIZE   |
| ^                                   | NK_ERROR__POOL_P_INVLD_STORAGE      |
| ^                                   | NK_ERROR__POOL_P_INVLD_NAME         |
| ^                                   | NK_ERROR__POOL_P_INVLD_FREE         |
| ^                                   | NK_ERROR__POOL_P_INVLD_TOTAL        |
| ^                                   | NK_ERROR__POOL_P_INVLD_OCCUPIED     |
| ^                                   | NK_ERROR__TIMER_P_INVLD_TIME_MS     |
| ^                                   | NK_ERROR__TIMER_P_INVLD_CALLBACK    |
| ^                                   | NK_ERROR__TIMER_P_INVLD_TIMER       |
| NK_ERROR__CALLED_FROM_ISR           | NK_ERROR__POOL_P_CALLED_FROM_ISR    |
| NK_ERROR__RESOURCE_BUSY             | NK_ERROR__POOL_P_IN_USE_DELETE      |
| NK_ERROR__NO_MEMORY                 | NK_ERROR__POOL_O_NO_FREE_POOL       |
| ^                                   | NK_ERROR__POOL_O_NO_FREE_BLOCK      |
| NK_ERROR__ILGL_CREATE               | NK_ERROR__POOL_S_ILGL_CREATE        |
| ^                                   | NK_ERROR__TIMER_S_ILGL_CREATE       |
| ^                                   | NK_ERROR__POOL_S_ILGL_DELETE        |

### Specific error generation

Component base values:

| Component name | Component base |
| -------------- | -------------: |
| pool           |      1000 0000 |
| timer          |      2000 0000 |
| flag           |      3000 0000 |
| queue          |      4000 0000 |
| mutex          |      5000 0000 |
| sem            |      6000 0000 |
| mbox           |      7000 0000 |
| task           |      8000 0000 |

Class base and class shift values:

| Class name               | Class base | Class shift |
| ------------------------ | ---------: | ----------: |
| precondition errors (P)  |  0010 0000 |          20 |
| postcondition errors (O) |  0000 4000 |          14 |
| safety errors (S)        |  0000 0100 |           8 |

The specific error code is generated by following formula:

    error_code = [component base] + [class base] + [index << class shift] + [general error]
    
Specific error values:

| Specific error                      | Component base | Class base | Class shift | Index | General error | Code      | 
| ----------------------------------- | -------------: | ---------: | ----------: | ----: | ------------: | --------: |
| NK_ERROR__POOL_P_INVLD_POOL         |      1000 0000 |  0010 0000 |          20 |     0 |            01 | 1010 0001 |
| NK_ERROR__POOL_P_INVLD_BLOCK        |              ^ |          ^ |           ^ |     1 |            01 | 1020 0001 |
| NK_ERROR__POOL_P_INVLD_N_BLOCKS     |              ^ |          ^ |           ^ |     2 |            01 | 1030 0001 |
| NK_ERROR__POOL_P_INVLD_BLOCK_SIZE   |              ^ |          ^ |           ^ |     3 |            01 | 1040 0001 |
| NK_ERROR__POOL_P_INVLD_STORAGE      |              ^ |          ^ |           ^ |     4 |            01 | 1050 0001 |
| NK_ERROR__POOL_P_INVLD_NAME         |              ^ |          ^ |           ^ |     5 |            01 | 1060 0001 |
| NK_ERROR__POOL_P_INVLD_FREE         |              ^ |          ^ |           ^ |     6 |            01 | 1070 0001 |
| NK_ERROR__POOL_P_INVLD_TOTAL        |              ^ |          ^ |           ^ |     7 |            01 | 1080 0001 |
| NK_ERROR__POOL_P_INVLD_OCCUPIED     |              ^ |          ^ |           ^ |     8 |            01 | 1090 0001 |
| NK_ERROR__POOL_P_CALLED_FROM_ISR    |              ^ |          ^ |           ^ |     9 |            06 | 10a0 0006 |
| NK_ERROR__POOL_P_IN_USE_DELETE      |              ^ |          ^ |           ^ |     a |            05 | 10b0 0005 |
| NK_ERROR__POOL_O_NO_FREE_POOL       |              ^ |  0000 4000 |          14 |     0 |            04 | 1000 4004 |
| NK_ERROR__POOL_O_NO_FREE_BLOCK      |              ^ |          ^ |           ^ |     1 |            04 | 1000 8004 |
| NK_ERROR__POOL_S_ILGL_CREATE        |              ^ |  0000 0100 |           8 |     0 |            03 | 1000 0103 |
| NK_ERROR__POOL_S_ILGL_DELETE        |              ^ |          ^ |           ^ |     1 |            03 | 1000 0203 |
| NK_ERROR__TIMER_P_INVLD_TIME_MS     |      2000 0000 |  0010 0000 |          20 |     0 |            01 | 2010 0001 |
| NK_ERROR__TIMER_P_INVLD_CALLBACK    |              ^ |          ^ |           ^ |     1 |            01 | 2020 0001 |
| NK_ERROR__TIMER_P_INVLD_TIMER       |              ^ |          ^ |           ^ |     2 |            01 | 2030 0001 |
| NK_ERROR__TIMER_O_NO_FREE_TIMER     |              ^ |  0000 4000 |          14 |     0 |            04 | 2000 4004 |
| NK_ERROR__TIMER_S_ILGL_CREATE       |              ^ |  0000 0100 |           8 |     0 |            03 | 2000 0103 |# Function classes

## Execution context

A function may be called from different execution contextes. We differentiate the following 
execution contextes:
- __thread execution__ - This execution context is started automatically in main function. Even when
  no RTOS is being used this context is still called `thread` context.
- __interrupt service execution__ - This context is present in interrupt service routines.

## Execution class

The thread context can be interrupted by interrupt context. In every application scenario there is
a need to share data between thread and interrupt context. In order to provide data consistency, a
some kind of data protection mechanism is being used (an interrupt lock or an RTOS construct, like 
mutex or semaphore) to protect and serialize the access to the data. 

In order to provide data protection mechanism we can have different levels of locking:
- __system lock__ - thread-to-thread context switching is disabled. Interrupts are still being
  serviced, but when they return, it is guranteed that the execution will return to the interrupted
  thread.
- __interrupt lock__ - all context switching is disabled. No Nano Kernel interrupts are serviced.
  This is the most restrictive type of lock and its usage should be minimised.
  
A function implementation usually doesn't always implement a data protection mechanism for data
access. It is expected from caller that it implements it in the most efficient way. In order to 
minimise wrong usage of functions, the function classes have been defined.

## Function classes

Depending on the data access aproach we distinguish the following function classes:
* __general function class__ - This class of functions provides all necessary data protection 
  mechanism when accessing shared data. These functions can be called from thread and interrupt 
  context without the need to implement specific data protection mechanism. Note that some functions
  do have a restriction to be called from interrupt context, however.
* __system lock function class__ or __S class__ - These functions are assuming that data is shared 
  exclusively between threads. When these kind of functions are accessing shared data, they assume 
  that thread context switching will not happen. This is usually ensured by `locking` the system 
  scheduler, hence the name `system lock` function class.
* __interrupt lock function class__ or __I class__ - These functions are assuming that the data is 
  shared between multiple threads and at least one interrupt service routine. When these kind of 
  functions are accessing shared data, they assume that thread context switching is disabled and 
  interrupt service routines are not allowed. In other words, these functions do not implement any 
  kind of data protection mechanism.
  
Some Nano Kernel components are providing the above versions of the functions. As a caller, the
application must ensure that:
- when a __S class__ function is called, that the Nano Kernel scheduler is locked.
- when a __I class__ function is called, that the Nano Kernel interrupts are disabled.
# Theory of operation

## Neon-Kit Event Driven System (EDS) project structure

The project contains the following folders:
- __build__: Contains scripts and configuration files for Doxygen and for unit-tests.
- __configuration__: Contains templates which are to be used by application to define EDS
  configuration options.
- __documentation__: Contains text documents, images and source files of images diagrams.
- __source__: Contains all the source code of EDS which is common to all supported ports.
- __tests__: Contains unit-tests.

# Sources

- `eds.h` - Main API header file. This is usually the only needed header file for application to
  use the EDS. It does not include additional header files besides standard C files. All types
  defined in this header are opaque types to objects. The application uses
  `eds__<object>__create_<variant>` functions to allocate objects.

  __Declares:__
  - Public API macros.
  - Public API integral types.
  - Public API opaque types.
  - Public API functions.
  - Object naming prefix: `eds__[object]__` and `EDS__[OBJECT]__`.

  __Restrictions:__
  - Must not include any other header file except standard C library header files.

  __Rationale:__
  - Since this file is main API header file, we don't want to include a bunch of header files to be
    included by it. This enhances understandability and encapsulation of the implementation. The
    goal is to have one header file and several implementation files for complete EDS
    implementation. Other header files in this directory are mainly used to extend the existing
    implementation of EDS.

- `eds_object.h` - Secondary API file. This file should not be included by application code, except,
  when the application wants to extend the objects or allocate the objects statically.

  __Declares:__
  - All object types used by main `eds.h` API header.
  - All object types used internally by EDS.
  - Object naming prefix: `eds_object__`.

  __Restrictions:__
  - Must not include any other header file except `eds.h`, `eds_config.h`, `eds_port.h` and
    standard C library header files.

  __Rationale:__
  - The cleanest approach would be that this header includes only standard C library header files.
    Unfortunately, this is impossible due to configurable nature of EDS. The application
    may configure the EDS as it sees fit and this requirement impacts how the objects are
    constructed, too.

- `eds_[component]_.h` - Private API file. This file should not be included by application code,
  except, when the application wants to reuse (or extend) some functinality of EDS implementation.

  __Declares:__
  - All internal component functions of EDS. All internal component functions have protected
    access level.
  - All global variables (all variables have protected access level to EDS).
  - Object naming prefix: `eds_[component]__` and `EDS_[COMPONENT]__`.

  __Restrictions:__
  - Must not include any other header file except `eds_object.h`, other `eds_[component].h` and
    standard C library header files.

  __Rationale:__
  - All internal component functions and variables have protected access level. This means that
    they are not part of public API, but might be accessed by including additional headers. This
    allows that an external component or application might use the existing functionality offered
    by EDS.

- `eds_[component].c` - Private implementation file.

  __Defines:__
  - All functions which are internal to EDS.
  - All global variables.

- `core` component (`eds_core.h` and `eds_core.c`) is the only component that implements more than
  one sub-components.# Time complexity

In computer science, the time complexity of an algorithm quantifies the amount
of time taken by an algorithm to run as a function of the length of the input.
The time complexity of an algorithm is commonly expressed using **big O**
notation, which excludes coefficients and lower order terms. When expressed this
way, the time complexity is said to be described asymptotically, i.e., as the
input size goes to infinity. For example, if the time required by an algorithm
on all inputs of size `n` is at most ` 5n^3 + 3n`, the
asymptotic time complexity is `O(n^3)`.

Time complexity is commonly estimated by counting the number of elementary
operations performed by the algorithm, where an elementary operation takes a
fixed amount of time to perform. Thus the amount of time taken and the number of
elementary operations performed by the algorithm differ by at most a constant
factor.

Since an algorithm’s performance time may vary with different inputs of the same
size, one commonly uses the worst-case time complexity of an algorithm, denoted
as **T(n)**, which is defined as the maximum amount of time taken on any
input of size `n`. Time complexities are classified by the nature of
the function `T(n)`. For instance, an algorithm with `T(n) =
O(n)` is called a linear time algorithm, and an algorithm with `T(n)
= O(2^n)` is said to be an exponential time algorithm.

__NOTE__:  

    Worst-case time-complexity `T(n)` indicates the longest running time
    performed by an algorithm given any input of size `n`, and thus this
    guarantees that the algorithm finishes on time.

## Big O notation

Big O notation describes the limiting behavior of a function when the argument
tends towards a particular value or infinity, usually in terms of simpler
functions and it is used to classify algorithms by how they respond (e.g., in
their processing time or working space requirements) to changes in input size.

##  Constant time
An algorithm is said to be constant time (also written as `O(1)` time) if the
value of `T(n)` is bounded by a value that does not depend on the size of the
input.

Despite the name *constant time*, the running time does not have to be
independent of the problem size, but an upper bound for the running time has to
be bounded independently of the problem size.

__NOTE__:  

    Constant time effectively means that there is a constant upper bound to
    how long the function will take to run which isn’t affected by any of the
    input argument.

## Nano Kernel time complexity

All Nano Kernel functions are using `constant time O(1)` algorithms. This is
especially important for Real Time applications.

