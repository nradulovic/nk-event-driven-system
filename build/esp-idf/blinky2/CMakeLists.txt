idf_component_register(
    SRCS 
        "../../../documentation/examples/blinky2/blinky2.c" 
    INCLUDE_DIRS 
        "../../../documentation/examples/blinky2" 
    REQUIRES 
        eds-generic)
                       
target_compile_options(${COMPONENT_TARGET} PRIVATE -Wall -Wextra)
