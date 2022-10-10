#include "sys/eds_epn.h"
#include "eds_port.h"

extern inline struct eds_object__tasker*
eds_epn__tasker(struct eds_object__epn * epn);

extern inline struct eds_object__etm_sentinel *
eds_epn__etm_service(struct eds_object__epn * epn);

void
eds_epn__sleep_init(struct eds_object__epn * epn)
{
#if (EDS_PORT__USE_LOCAL_SLEEP == 1)
    eds_port__sleep_local_init(&epn->p__sleep);
#else
    eds_port__sleep_global_init();
#endif /* (EDS_PORT__USE_LOCAL_SLEEP == 1) */
}

void
eds_epn__sleep_wake_up(struct eds_object__epn * epn)
{
#if (EDS_PORT__USE_LOCAL_SLEEP == 1)
    eds_port__sleep_local_signal(&epn->p__sleep);
#else
    eds_port__sleep_global_signal();
#endif /* (EDS_PORT__USE_LOCAL_SLEEP == 1) */
}

void
eds_epn__sleep_wait(struct eds_object__epn * epn)
{
#if (EDS_PORT__USE_LOCAL_SLEEP == 1)
    eds_port__sleep_local_wait(&epn->p__sleep);
#else
    eds_port__sleep_global_wait();
#endif /* (EDS_PORT__USE_LOCAL_SLEEP == 1) */
}
