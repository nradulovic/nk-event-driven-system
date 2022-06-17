
#include "sys/eds_state.h"
#include "sys/eds_core.h"

static struct eds_object__mem mem__instances_storage[EDS__DEFAULT_MEM_ENTRIES];

struct eds_object__vector eds_state__mem_instances = EDS_CORE__VECTOR__INITIALIZER(mem__instances_storage);

bool eds_state__has_started = false;
