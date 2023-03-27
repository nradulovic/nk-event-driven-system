# Neon Kit

This module is part of Neon Kit.

## Architecture of Neon Kit

Neon Kit consists of the following modules:
- Event Driven System (EDS)
- Real-time kernel (RTK)
- Embedded Principle Library (EPL)
- Shared Build System (SBS)
- Unit Test Framework (UTF)

Each part is contained in a separate git repository. This allows application writer to assemble a 
Neon Kit that suits his needs.

# Neon Kit - Event Driven System

This module contains code which is responsible for event handling using state machines. The code 
is split into two sections:
- generic
- portable

This approach provides high portability to various systems.

## Generic code

The generic code is a code which does not depend on used CPU architecture, peripherals and compilers. 
A particular combination of CPU architecture, peripherals and compiler is called a port. This code 
is shared across all supported ports.

## Portable code

This code is something that is used by application writer. The application writer is 
responsible to implement all needed functionality of a port. Port code is located in `portable` 
directory. For each supported combination of compiler (platform), CPU architecture, CPU family
and operating system there exists a folder with neede portable code.

## Cloning this repository

This repo does no use Git Submodules to bring in dependent modules.

To clone using HTTPS:

    git clone https://github.com/nradulovic/nk-event-driven-system.git

Using SSH:

    git clone git@github.com:nradulovic/nk-event-driven-system.git

## Project directory structure

The project contains the following folders:
- `build` - this directory contains a number of Makefile, CMake and Doxygen files which are 
  used by unit-testing framework and documentation build system. This directory was not 
  intended to be used in application build system, but it could be used for that purposes 
  as well. The makefiles in this directory are used to build unit-tests, documentation (HTML 
  and PDF) and examples which are located in `documentation/examples` directory. Any build 
  artifact is stored in new directory called `generated` in project top level directory.
- `documentation` - contains various descriptions of the system, how to use it properly and
  some examples in `documentation/examples`.
- `source` - this directory contains all the source files. This is the only directory that is
  needed by application.
- `portable` - contains portable files which are needed by Even Driven System.
- `tests` - contains unit-tests and profiling tests.

## Unit-testing

Neon Kit Event Driven system contains unit-tests in `tests` directory. To start unit-testing refer
to [documentation/DEVELOPER_MANUAL.md](documentation/DEVELOPER_MANUAL.md) for details.

## Where is a build system?

There is no default build system for Neon Kit suite modules. The Neon Kit is intended to be used
like a plugin to application project and it does not favor any build system. It is up to application 
writer to integrate Neon Kit modules into the project. Since there are so many IDEs and build 
systems it is only natural to leave the building process to application writer. The only thing 
that a Neon Kit module expect is a directory structure specified in the description following this chapter.

## Building a Neon Kit module

All Neon Kit modules need to be placed into a single directory. For convenience lets refer to
this directory as a variable named `NEON_KIT_ROOT`.

The following are needed to be configured in application project:
- include path should have the following entries:
  - directory where all Neon Kit projects are located in `$NEON_KIT_ROOT`.
  - directory where a specific Neon Kit module is located: 
    `$NEON_KIT_ROOT/[module]/source`. In this case the path would be:
    `$NEON_KIT_ROOT/eds/source`.
  - directory where portable definition `[platform-architecture-family-os]` is located.
- no defined macros are needed except when a custom module configuration is needed which is 
  explained below.
- compile all sources under `source` directory and selected portable directory, in this case
 `$NEON_KIT_ROOT/eds/source`.

### Example

Lets say that we have a firmware application called `application_1` with the following structure:
1. The firmware application is located in `firmware/application_1` folder.
2. In this application Neon Kit EDS module is used and is located in `firmware/application_1/lib/neon-kit/eds` folder.
3. The portable definition is located in `firmware/application_1/lib/neon-kit/eds/portable/gcc-any-any-freertos`. 

With this setup we would need the following additional include paths:
- `firmware/application_1/lib/neon-kit`
- `firmware/application_1/lib/neon-kit/eds/source`
- `firmware/application_1/lib/neon-kit/eds/portable/gcc-any-any-freertos`

## Building Event Driven System with custom configuration

When a custom configuration is not enabled, Event Driven System uses default configuration which 
should be a good fit for most applications. It is possible to create a specific Event Driven System
implementation which is optimal for the current application. This feature is by default disabled. To
enable using custom configuration the build system must define the `EDS_CONFIG__ENABLE` macro. When
this macro is defined by the build system, then Event Driven System will include additional header
file called `eds_config_definition.h`. Use this file to define the custom project configuration.

## Application Programming Interface

The application programming interface is governed by 
[coding style guide](documentation/DEVELOPER_MANUAL.md) document. The document defines:

- __Coding style__: All public objects declared in Application Programming Interface are following
  the coding style defined in [coding style document](documentation/DEVELOPER_MANUAL.md).
- __Naming rules__: Names of functions, types and variables are defined in the coding style guide
  document, too. Naming abbreviations are defined by [abbreviations](documentation/USER_MANUAL.md) 
  document.
- __Common exception or error reporting__: All Neon Kit API functions are handling exceptions and
  errors in the same manner. Assertions are not used in great number which allows application writer 
  to decide what happens on certain errors and exceptions (does it want to handle the error or 
  exception or it wants to stop the application execution). Error and exception checking in Nano Kit
  is divided into the following categories:
  - __precondition exceptions__: Precondition exceptions are generated when function preconditions 
    are not satisfied. Some examples include, but are not limited to, when passing invalid arguments 
    to a function, a function is called from improper context or when function input arguments are 
    in invalid/illegal state(s). 
  - __postcondition exceptions__: Postcondition exceptions are generated when function can not 
    fulfill its designated task. Some examples are, but not limited to, failing to allocate memory 
    or a resource, failed to send data and etc.
  - __safety exceptions__: Safety exceptions are generated when functions asserts certain details 
    about the input objects or when the functions is called from inappropriate application state. 
    Some examples, include, but are not limited to, when an allocation function is called outside 
    the initialization state. If the application is not developed for a safety critical mission then 
    this exception handling might never be enabled. 
  - __general errors__: General errors are errors which can not be put into none of the above 
    categories. These errors can't be disabled, they are always enabled. Note that these errors 
    don't have the module name field in them as the exceptions in other categories.

  For more details on exception and error handling refer to 
  [error handling](documentation/DEVELOPER_MANUAL.md) document.
- __Function classes__: Functions are divided into classes regarding the execution context. The
  function class is part of the function name so there should be no confusion when a function may
  be called. For details see [function classes](documentation/DEVELOPER_MANUAL.md) document.

## Deterministic

Majority of algorithms used in Neon Kit are belonging to **Constant Time Complexity** category. 
Constant Time `O(1)` functions needs fixed amount of time to execute an algorithm. In other words 
the execution time of Constant Time Complexity functions does not depend on number and/or value of 
inputs. For more information see [time complexity](documentation/USER_MANUAL.md) document.

## Configurable

The Neon Kit uses configuration files which can be used to tailor the implementation to application 
needs.

In addition, the Neon Kit modules implements a number of hooks which can alter or augment the 
behavior of the module, by allowing application to intercept function calls between software 
modules.

## Portable

During the design stage of the module a special attention was given to achieve high portability. 
Some data structures and algorithms are tailored to exploit new hardware features.

## Static object allocation
All objects used in Neon Kit can be statically allocated. There is no need to use any memory 
management functionality which makes it suitable to verify the application using static analysis 
tools.

## Up to 32 Agent priority levels
Each Agent has a defined priority. Lowest priority level is 0, while the highest available is level 
31.

## Application development guides

Several application development approaches are defined when using Neon Kit. More details on how
to structure the project are available in 
[application development](documentation/DEVELOPER_MANUAL.md) document. 


