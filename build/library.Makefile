# Relative path to Neon-Kit common build directory.
WS = ..

# Project name, this will be used as output binary file name.
PROJECT_NAME := event-driven-system

# Project configuration header file.
PROJECT_CONFIG := 

# List additional C header include paths.
CC_INCLUDES += source

# List additional C source files. Files which are not listed here will not be
# compiled.
CC_SOURCES += source/eds.c
CC_SOURCES += source/sys/eds_core.c
CC_SOURCES += source/sys/eds_epa.c
CC_SOURCES += source/sys/eds_epn.c
CC_SOURCES += source/sys/eds_equeue.c
CC_SOURCES += source/sys/eds_etm.c
CC_SOURCES += source/sys/eds_evt.c
CC_SOURCES += source/sys/eds_mem.c
CC_SOURCES += source/sys/eds_smp.c
CC_SOURCES += source/sys/eds_state.c
CC_SOURCES += source/sys/eds_tmr.c

# List additional archives. Use this when using an external static archive.
AR_LIBS +=

# List additional libraries. Use this when using an external static library.
LD_LIBS +=

# List additional features
VARIANT_FEATURES += 

# Include initialization of build system, this include should be the first
include $(WS)/../shared-build-system/initialize.mk

# Include application dependencies

# Include target of build system, a target is the last include in the Makefile
include $(WS)/../shared-build-system/target_library.mk
