idf_component_register(
    SRCS 
        "../../../documentation/examples/blinky/blinky.c" 
    INCLUDE_DIRS 
        "../../../documentation/examples/blinky" 
    REQUIRES 
        eds-generic)
                       
target_compile_options(${COMPONENT_TARGET} PRIVATE -Wall -Wextra)
