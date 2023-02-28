#include "sys/eds_epn.h"
#include "eds_port.h"

extern inline struct eds_object__tasker*
eds_epn__tasker(struct eds_object__epn * epn);

extern inline struct eds_object__etm_sentinel *
eds_epn__etm_service(struct eds_object__epn * epn);

extern inline struct eds_port__sleep *
eds_epn__sleep(struct eds_object__epn * epn);
