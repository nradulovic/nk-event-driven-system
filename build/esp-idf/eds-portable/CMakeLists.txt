idf_component_register(
    SRCS 
        "../../../portable/gcc-any-any-freertos/eds_port/eds_port_definition.c"
    INCLUDE_DIRS 
        "../../../portable/gcc-any-any-freertos"
        "."
    REQUIRES 
        freertos)
                       
target_compile_options(${COMPONENT_TARGET} PUBLIC -DESP_IDF PRIVATE -Wall -Wextra)
