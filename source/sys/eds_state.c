
#include "sys/eds_state.h"
#include "sys/eds_core.h"

static struct eds_object__mem mem__instances_storage[EDS__DEFAULT_MEM_ENTRIES];

struct eds_object__list eds_state__epn_instances = EDS_CORE__LIST_INITIALIZER(&eds_state__epn_instances);
struct eds_object__vector eds_state__mem_instances = EDS_CORE__VECTOR__INITIALIZER(mem__instances_storage);
struct eds_object__tmr_sentinel eds_state__tmr_sentinel;
bool eds_state__has_started = false;
bool eds_state__is_port_initialized = false;
