idf_component_register(
    SRCS 
        "../../source/eds.c" 
        "../../source/sys/eds_core.c" 
        "../../source/sys/eds_epa.c"
        "../../source/sys/eds_epn.c"
        "../../source/sys/eds_equeue.c"
        "../../source/sys/eds_etm.c"
        "../../source/sys/eds_evt.c"
        "../../source/sys/eds_mem.c"
        "../../source/sys/eds_smp.c"
        "../../source/sys/eds_state.c"
        "../../source/sys/eds_tmr.c"
        "../../portable/gcc-any-any-freertos/eds_port/eds_port_definition.c"
    INCLUDE_DIRS 
        "../../source" 
        "../../portable/gcc-any-any-freertos"
    REQUIRES 
        freertos)
                       
target_compile_options(${COMPONENT_TARGET} PRIVATE -Wall -Wextra -pedantic)
