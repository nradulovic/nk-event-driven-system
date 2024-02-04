/*
 * eds_epa.h
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_EDS_EPA_H_
#define NEON_KIT_GENERIC_SOURCE_EDS_EPA_H_

#include <stdbool.h>

#include "eds_object.h"
#include "sys/eds_core.h"

inline void
eds_epa__designate(struct eds_object__epa * epa, struct eds_object__epn * epn)
{
    epa->p__epn = epn;
}

inline struct eds_object__epn *
eds_epa__designation(const struct eds_object__epa * epa)
{
    return epa->p__epn;
}

inline bool
eds_epa__is_designated(const struct eds_object__epa * epa)
{
    return epa->p__epn != NULL;
}

inline const struct eds_object__smp *
eds_epa__smp(const struct eds_object__epa * epa)
{
    return &epa->p__smp;
}

inline struct eds_object__epa *
eds_epa__from_tasker_node(struct eds_object__tasker_node * epa)
{
    return EDS_CORE__CONTAINER_OF(epa, struct eds_object__epa, p__task);
}

eds__error
eds_epa__create(eds__sm_state * sm_initial_state,
    void * sm_workspace,
    const struct eds__agent_attr * attr,
    struct eds_object__epa ** epa);

eds__error
eds_epa__send_initial_event(struct eds_object__epa * epa);

eds__error
eds_epa__send(struct eds_object__epa * epa, const struct eds_object__evt * evt);

eds__error
eds_epa__dispatch(struct eds_object__epa * agent);

void
eds_epa__flush_events(struct eds_object__epa * epa);

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_EPA_H_ */
