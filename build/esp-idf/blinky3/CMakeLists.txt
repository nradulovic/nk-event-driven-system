idf_component_register(
    SRCS 
        "../../../documentation/examples/blinky3/blinky3.c" 
    INCLUDE_DIRS 
        "../../../documentation/examples/blinky3" 
    REQUIRES 
        eds-generic)
                       
target_compile_options(${COMPONENT_TARGET} PRIVATE -Wall -Wextra)
