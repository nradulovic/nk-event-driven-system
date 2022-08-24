# Relative path to Neon-Kit common build directory.
WS = ..

# Project name, this will be used as output binary file name.
PROJECT_NAME := test_eds_mem

# Project configuration header file.
PROJECT_CONFIG := tests/unit-tests/$(PROJECT_NAME)/eds_port

# List additional C header include paths.
CC_INCLUDES += tests/unit-tests/$(PROJECT_NAME)

# List additional C source files. Files which are not listed here will not be
# compiled.
CC_SOURCES += tests/unit-tests/$(PROJECT_NAME)/$(PROJECT_NAME).c
CC_SOURCES += source/sys/eds_mem.c

# List additional archives. Use this when using an external static archive.
AR_LIBS +=

# List additional libraries. Use this when using an external static library.
LD_LIBS +=

# List additional features
VARIANT_FEATURES += 

# Include initialization of build system, this include should be the first
include $(WS)/build/../external/sbs/initialize.mk

# Include application dependencies
include $(WS)/build/eds/interface.mk
include $(WS)/build/eds/utf_interface.mk
include $(WS)/$(DEF_EXTERNAL_DIR)/utf/build/utf.mk

# Include target of build system, a target is the last include in the Makefile
include $(WS)/build/../external/sbs/target_test.mk

