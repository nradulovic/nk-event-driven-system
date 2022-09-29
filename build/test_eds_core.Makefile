# Relative path to Neon-Kit common build directory.
WS = ..

# Project name, this will be used as output binary file name.
PROJECT_NAME := test_eds_core

# Project configuration header file.
PROJECT_CONFIG := tests/unit-tests

# List additional C header include paths.
CC_INCLUDES += 

# List additional C source files. Files which are not listed here will not be
# compiled.
CC_SOURCES += tests/unit-tests/$(PROJECT_NAME)/$(PROJECT_NAME).c
CC_SOURCES += source/sys/eds_core.c

# List additional archives. Use this when using an external static archive.
AR_LIBS +=

# List additional libraries. Use this when using an external static library.
LD_LIBS +=

# List additional features
VARIANT_FEATURES += 

# Include initialization of build system, this include should be the first
include $(WS)/../shared-build-system/initialize.mk

# Include application dependencies
include $(WS)/build/interface.mk
include $(WS)/../unit-testing-framework/build/library.mk

# Include target of build system, a target is the last include in the Makefile
include $(WS)/../shared-build-system/target_test.mk

