idf_component_register(
    SRCS 
        "../../../source/eds.c" 
        "../../../source/sys/eds_core.c" 
        "../../../source/sys/eds_epa.c"
        "../../../source/sys/eds_epn.c"
        "../../../source/sys/eds_equeue.c"
        "../../../source/sys/eds_etm.c"
        "../../../source/sys/eds_evt.c"
        "../../../source/sys/eds_mem.c"
        "../../../source/sys/eds_smp.c"
        "../../../source/sys/eds_state.c"
        "../../../source/sys/eds_tmr.c"
    INCLUDE_DIRS 
        "../../../source"
        "."
    REQUIRES 
        eds-portable)
                       
target_compile_options(${COMPONENT_TARGET} PRIVATE -DEDS_CONFIG__ENABLE -Wall -Wextra -pedantic)
