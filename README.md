# Neon Kit Generic

Neon Kit - Generic sources

## Architecture of Neon Kit

Neon Kit consists of the following parts:
- generic
- kernel
- platform
- psp

Each part is contained in a separate git repository. This allows application writter
to assemble a Neon Kit that suits his needs.

### Generic layer

This layer contains code which in common for all supported architectures, MCUs and
compilers.

### Kernel layer

Contains code for used architecture and OS.

### Platform layer

Contains code for used compiler.

### Peripheral Support Package (PSP) layer

Contains code which is specific to a certain MCU.

## Where is a build system?

There is no build system for Neon Kit suite. The Neon Kit contains only sources
which are needed to build the system. Since Neon Kit is like a plugin to 
application project it does not use or favor a build system. It is up to
application writer to intergrate Neon Kit into the project. Since there are so
many IDEs and build systems it is only natural to leave the build to application
writter. The only thing that Neon Kit expects is a directory structure specified 
in the description following this chapter.

## Building Neon Kit

The Neon Kit does not contain a build system. The Neon Kit contains only sources
which are needed to build the system. Since Neon Kit is like a plugin to 
application project it does not use or favor a build system. It is up to
application writer to intergrate Neon Kit into the project. Since there are so
many IDEs and build systems it is only natural to leave the build to application
writter. The only thing that Neon Kit expects is a directory structure containing
the Neon Kit parts.