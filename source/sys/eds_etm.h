/*
 * eds_etimer.h
 *
 *  Created on: Dec 12, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_EDS_SOURCE_EDS_ETIMER_H_
#define NEON_KIT_EDS_SOURCE_EDS_ETIMER_H_

#include <stdbool.h>

#include "eds_object.h"

#define EDS_ETIMER__EVENT__TICK     16

inline void
eds_etm__designate(struct eds_object__etm_node *etm, struct eds_object__epa *epa)
{
    etm->p__epa = epa;
}

inline bool
eds_etm__is_designated(const struct eds_object__etm_node *etm)
{
    return etm->p__epa != NULL;
}

eds__error
eds_etm__create(const struct eds__etimer_attr *attr, eds__etimer **etimer);

void
eds_etm_service__init(struct eds_object__etm *etm_service);

void
eds_etm_service__start_once(struct eds_object__etm *etm_service,
    struct eds_object__etm_node *etm,
    uint32_t ms);

void
eds_etm_service__start_periodic(struct eds_object__etm *etm_service,
    struct eds_object__etm_node *etm,
    uint32_t ms);

void
eds_etm_service__cancel(struct eds_object__etm *etm_service, struct eds_object__etm_node *etm);


void
eds_etm_service__tick(struct eds_object__etm *etm_service);

#endif /* NEON_KIT_EDS_SOURCE_EDS_ETIMER_H_ */
