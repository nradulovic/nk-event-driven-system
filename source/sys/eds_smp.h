/*
 * eds_sm.h
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_EDS_SM_H_
#define NEON_KIT_GENERIC_SOURCE_EDS_SM_H_

#include "eds_object.h"
#include "sys/eds_core.h"


extern const struct eds_object__evt g__smp_events[];

void
eds_smp__init(struct eds_object__smp * sm, eds_object__smp_state * initial_state, void * workspace);

eds__error
eds_smp__dispatch(struct eds_object__smp * sm, const struct eds_object__evt * event);

#define eds_smp__workspace(sm)              ((sm)->p__workspace)

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_SM_H_ */
