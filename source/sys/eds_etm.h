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

void
eds_etm_service__delete_all(struct eds_object__epa * epa);

void
eds_etm_service__init(void);

#endif /* NEON_KIT_EDS_SOURCE_EDS_ETIMER_H_ */
