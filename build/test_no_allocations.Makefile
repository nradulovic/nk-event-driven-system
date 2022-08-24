# Relative path to Neon-Kit common build directory.
WS = ..

# Project name, this will be used as output binary file name.
PROJECT_NAME := test_no_allocations

# Project configuration header file.
PROJECT_CONFIG := tests/integration-tests/test_no_allocations

# List additional C header include paths.
CC_INCLUDES += 

# List additional C source files. Files which are not listed here will not be
# compiled.
CC_SOURCES += tests/integration-tests/test_no_allocations/main.c
CC_SOURCES += tests/integration-tests/test_no_allocations/eds_port/eds_port_definition.c

# List additional archives. Use this when using an external static archive.
AR_LIBS +=

# List additional libraries. Use this when using an external static library.
LD_LIBS +=

# List additional features
VARIANT_FEATURES += pthread

# Include initialization of build system, this include should be the first
include $(WS)/build/../external/sbs/initialize.mk

# Include application dependencies
include $(WS)/build/eds/interface.mk
include $(WS)/build/eds/implementation.mk

# Include target of build system, a target is the last include in the Makefile
include $(WS)/build/../external/sbs/target_test.mk

