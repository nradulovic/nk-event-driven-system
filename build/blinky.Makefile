# Relative path to Neon-Kit common build directory.
WS = ..

# Project name, this will be used as output binary file name.
PROJECT_NAME := blinky

# Project configuration header file.
PROJECT_CONFIG := documentation/examples/blinky

# List additional C header include paths.
CC_INCLUDES +=

# List additional C source files. Files which are not listed here will not be
# compiled.
CC_SOURCES += documentation/examples/blinky/main.c

# List additional archives. Use this when using an external static archive.
AR_LIBS +=

# List additional libraries. Use this when using an external static library.
LD_LIBS +=

# List additional features
VARIANT_FEATURES += pthread

# Include initialization of build system, this include should be the first
include $(WS)/../shared-build-system/initialize.mk

# Include application dependencies
include $(WS)/build/library.mk

# Include target of build system, a target is the last include in the Makefile
include $(WS)/../shared-build-system/target_runnable.mk

