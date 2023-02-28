#ifndef NEON_KIT_EDS_SOURCE_EDS_NETWORK_H_
#define NEON_KIT_EDS_SOURCE_EDS_NETWORK_H_

#include "eds_object.h"

inline struct eds_object__tasker *
eds_epn__tasker(struct eds_object__epn * epn)
{
    return &epn->p__tasker;
}

inline struct eds_object__etm_sentinel *
eds_epn__etm_service(struct eds_object__epn * epn)
{
    return &epn->etm_sentinel;
}

inline struct eds_port__sleep *
eds_epn__sleep(struct eds_object__epn * epn)
{
    return &epn->p__sleep;
}

#endif /* NEON_KIT_EDS_SOURCE_EDS_NETWORK_H_ */
