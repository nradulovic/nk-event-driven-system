idf_component_register(
    SRCS 
        "../../../documentation/examples/blinky/main.c"
        "../../../documentation/examples/blinky/blinky.c"
        "../../../documentation/examples/blinky/std_allocator.c" 
    INCLUDE_DIRS 
        "../../../documentation/examples/blinky" 
    REQUIRES 
        eds-generic)
                       
target_compile_options(${COMPONENT_TARGET} PRIVATE -Wall -Wextra)
