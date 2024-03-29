# Developer manual

## Intro

__Intended audience__ of this document: 

* Developers who will work on Neon Kit sources (required).
* Testers who test Neon Kit sources (informative).
* Users of the Neon Kit (informative).

This document describes coding style guidelines and rules used in Neon Kit sources.

## About This Guide

Purpose of this style guide is to encourage use of common coding practices within the Neon Kit.

Style guide is a set of rules which are aimed to help create readable and maintainable code. 
By writing code which looks the same way across the code base will help others read and understand 
the code. By using the same conventions for spaces, newlines and indentation chances are reduced 
that future changes will produce huge unreadable diffs. By following the common patterns for code
structure and by using language features consistently will help others understand the code behavior.

## Abstract software granularity hierarchy

If we are to abstract from particular languages, frameworks and their own interpretations, the 
abstract software granularity hierarchy is the following:

    Product - application, library, service
      Module - GUI, core logic, data, etc...
        Component - purpose specific collection of objects
          Object - collection of primitives
            Primitive - numbers, functions, etc...

### Product

Plain and simple, the Product is a working collection of connected functional modules.

### Module

As the very name implies, the motivation of a Module is modularity. Contrary to what many claim, 
it does not really imply code reuse. There are many modules which are not really reusable, and 
don't fit with anything they were not designed to.

It is important to separate different software layers, that makes software much easier to implement 
and maintain, and should the need to reimplement something like a front end to a different GUI 
framework, modularity enables that to happen in an easy and safe manner, without breaking code all 
over the place.

A module encapsulates a collection of components which all serve a common purpose as defined by the 
module requirements. A module should be self-contained and complete, and while not really usable on 
its own, it should be able to work in conjunction with any conforming implementation.

### Component

In terms of granularity the Component sits between the Module and the Object. The purpose of a 
component is to put together a collection of general purpose objects to form a purpose specific unit.

As the name implies, unlike the Module, the Component is not "self-contained", it is a part of a 
larger functional whole.

### Object

Objects are the smaller building blocks of components. Objects are collections of primitives and 
couple them together to serve a lower level, more universal while still somewhat specific purpose.

### Primitive

Primitives are the smallest, simplest and lowest level of software development granularity. It's 
basically just integer and real numbers and functions/operators, although most languages have their 
own additional "first class citizens".

There is very little you can do with primitives, and at the same time, it is at a such a low level 
that you can accomplish practically everything with it. It is just very, very verbose, insanely 
complicated and impossibly tedious to accomplish while directly working with primitives.

# Naming rules

## General naming rules
- __R0100__: All functions, variables and types are using lower case names, words are delimited by 
  underscore `_` character. 
  Examples: 
    
      void do_something(void);
      uint32_t some_variable;
      
- __R0101__: All macro names are in `UPPERCASE` style, words are delimited by underscore `_` 
  character. 
- __R0102__: All functions, variables and types are prefixed with module prefix: `eds_`, `rtk_`, `ep_`.
- __R0103__: All typedefs are named in the same manner as struct or union types. No suffixes (like 
  the usual `_t` suffix) are allowed.
- __R0104__: All macro names are prefixed with module prefix: `EDS_`, `RTK_`, `EP_`.
- __R0105__: In addition to __R0102__, the global public variables are additionally prefixed with 
  `__g__`, resulting in `eds__g__` prefix.
- __R0106__: In addition to __R0102__, the global local (file scope) variables are additionally 
  prefixed with `__l__`, resulting in `eds__l__` prefix.
- __R0107__: In addition to __R0102__, the global private variables or functions are additionally 
  prefixed with `__p__`, resulting in `eds__p__` prefix.
- __R0108__: The name of API functions is constructed in the following way:

      [module]__[component]_[function_name]
      
  Where:
  - `[module]` is standard prefix as defined by __R0102__.
  - `[component]` is the name of the component which function is part of.
  - `__` is component and function name separator.
  - `[function_name]` is the name of the function.
  
  Examples: 
  - In the following example, the function name is  `do_something` and it is part of API for `list` 
    component:
    
        void eds__list_do_something(void);
        
  - The function name is `cancel` and it is part of API for `timer` component:
  
        void eds__timer_cancel(eds__timer * timer);
        
  - The function name is `long_name_operation` and it is part of API for `long_name_component` 
    component:
    
        void eds__long_name_component_long_name_operation(void);

## Function classes naming rules

For details on what are function classes, refer to [function classes document](function_classes.md).

All API functions may be suffixed if they fall are into either I or S class categories:
- __R0200__: - I class - Regular Interrupts are locked. Function suffix is `_i`.
- __R0201__: - S class - Scheduler switchings are locked. Function suffix is `_s`.

## Portable objects naming rules

All Port Interface objects are named using the rules stated above with certain differences:
- __R0300__: In addition to __R0102__, all functions, variables and types are additionally prefixed 
  with: `port_`, resulting in `nk_port_` prefix.
- __R0301__: In addition to __R0104__, all macro names are additionally prefixed with: `PORT_`, 
  resulting in `NK_PORT_` prefix.
- __R0302__: In addition to __R0102__, all port specific functions, variables and types (which are 
  not part of public NK RTOS API) are additionally prefixed with `cpu_`, resulting in `nk_cpu_` 
  prefix.
- __R0303__: In addition to __R0104__: all port specific macros are additionally prefixed with 
  `CPU_`, resulting in `NK_CPU_` prefix.

## Structure and union member naming rules

Members which are private and not to be accessed and modified by application code must follow the 
following naming rules:
- __R0400__: Private members must use `p__` prefix in order to tell to programmer that the member is 
  private to the structure.
  
# Right margin

- __R0500__: The hard wrap is set to column 100.

# Indentation

- __R0600__: Macro body starts either on column 45 if the body fits in between the column and the 
  right ruler defined by __R0500__. If the macro body does not fit or the macro name is too long, 
  the macro body starts on next line with  `\` continuation character located one tab before the
  right margin defined by __R0500__ rule. The body on next line start at column 9 (two tabs).
  
      #define NK_SAFETY__HAS_STARTED()            (nk__p__safety_critical_has_started == true)
      
      #define NK_SAFETY__START()                                                                      \
              do {                                                                                    \
                  nk__p__safety_critical_has_started = true;                                          \
              } while (0)

              
# Fixed or predefined values

Never code a fixed value directly in code. Always use a macro for this job:

    #define SOME_LIMIT_CONDITION 23

    uint32_t configurable_size_array[SOME_LIMIT_CONDITION];


# Comments

The Rob Pike article "Notes on programming in C" section about comments is spot on:

    A delicate matter, requiring taste and judgement. I tend to err on the side of
    eliminating comments, for several reasons. First, if the code is clear, and
    uses good type names and variable names, it should explain itself.  Second, 
    comments aren't checked by the compiler, so there is no guarantee they're 
    right, especially after the code is modified.  A misleading comment can be very
    confusing. Third, the issue of typography: comments clutter code.

    But I do comment sometimes. Almost exclusively, I use them as an introduction
    to what follows. Examples: explaining the use of global variables and types 
    (the one thing I always comment in large programs); as an introduction to an
    unusual or critical procedure; or to mark off sections of a large computation.

# Error handling

Error handling in Neon Kit is using _return value_ method. 
- __G0100__: The overall look of functions needs to comform to the following style:

      int function(void)
      {
          int error;
          error = do_something1();
      
          if (error) 
              goto CLEAN_1;
              
          error = do_something2();
      
          if (error)
              goto CLEAN_2;
            
          error = do_something3();
      
          if (error)
              goto CLEAN_3;
            
          return ERR_NONE;
      CLEAN_3:
          undo_something2();
      CLEAN_2:
          undo_something1();
      CLEAN_1:
          return error;
      }
    
- __G0101__: If a function does not need to undo actions, the following style is also acceptable:

      int function(void)
      {
          int error;
          error = do_something1();
      
          if (error) {
              return error;
          }
          error = do_something2();
      
          if (error) {
              return error;
          }
          error = do_something3();
      
          if (error) {
              return error;
          }
          return ERR_NONE;
      }
    
- __G0102__: If a function needs only to ensure that arguments are valid the following style should 
  be used:

      int function(arg_type arg1, arg_type arg2, arg_type arg3)
      {
          if (IS_NOT_VALID(arg1)) {
              return NK_ERROR__X__X1;
          } 
          if (IS_NOT_VALID(arg2)) {
              return NK_ERROR__X__X2;
          }
          if (IS_NOT_VALID(arg3)) {
              return NK_ERROR__X__X3;
          }

          /* ... */
                    
          return NK_ERROR__NONE;
      }
# Development pull requests (PR)

![Pull request use-case](images/pull_request_use_case.png)

The Author/Reviewer(s)/Repo admin workflow is the following:

1. When __Author__ starts working on a task it first creates a branch.
2. While working on the task it pushes commits to the branch created in step 1.
3. When the __Author__ completes the task it creates a PR with a description and specifies the 
   reviewers in the __Reviewers__ group. This creates a notification for involved reviewers. 
   __Author__ and __Repo Admin__ are waiting on __Reviewers__ approval(s).
4. Once notified, all reviewers review the commits by reading the code.
5. If a reviewer from __Reviewers__ group find an issue it will create a change request. This is 
   done by creating a comment for code or file. This creation notifies __Author__ that it needs to 
   take an action.
6. __Author__ implements changes and pushes the change commits on the same branch.
7. __Reviewers__ review the commits again and confirms the issue is corrected by closing the change 
   request.
8. When __Reviewers__ finds no more issues it will approve the PR. This generates notification for 
   __Repo Admin__. At this point __Repo Admin__ will evaluate if all mandatory reviews and approvals 
   are in place. If not, it still waits for reviews and approvals of other reviewers in 
   __Reviewers__.
9. During a code review a merge conflict may arrise. This merge conflict is detected by repository 
   and the repository notifies the __Author__.
10. __Author__ implements merge fixes and pushes the merge commits to the same branch.
11. Once merge conflicts are resolved the repository notifies the __Author__.
12. __Author__ has completed all merge fixes and then resets the review(s). This creates another 
    batch of notification for __Reviewers__ and __Repo Admin__. Since the review process is 
    restarted actors go through steps 4 to 8.
13. Once all issues are resolved and all reviews and approvals are in place __Repo Admin__ may 
    initiate merge procedure. Once the merging is done the repository will notify __Author__.
14. __Author__ closes the task and proceedes to delete the branch created in step 1. This step 
    completes the PR procedure.

# Building HTML or PDF documentation

This component uses Doxygen comments in code to build additional HTML or PDF
documentation.

## Developer machine setup

In order to execute unit-tests the following criteria must be met:

1. Using Ubuntu/Debian machine
2. GNU make is installed
3. Doxygen is installed
4. Preferably `dot` utility is installed

# Testing Neon-Kit Event Driven System implementation

Along Event Driven System sources unit-tests are bundled in `tests` directory.

Unit-testing can be run in the following ways:
- natively on Ubuntu machine
- using Docker Ubuntu virtual machine

## Testing on Ubuntu machine

In order to execute unit-tests do the following:

    sudo apt-get install git make gcc
    git submodule update --init --force --recursive
    cd build
    make test

## Testing using Docker

The main docker file is `Dockerfile` in top-level project directory. Start the
testing using this dockerfile. On a linux machine do:

    make test

# Version information

Given a version number MAJOR.MINOR.PATCH, increment the:

* MAJOR version when you make incompatible API changes,
* MINOR version when you add functionality in a backwards compatible manner, and
* PATCH version when you make backwards compatible bug fixes.

Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.PATCH format.
