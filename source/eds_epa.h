/*
 * eds_epa.h
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_EDS_EPA_H_
#define NEON_KIT_GENERIC_SOURCE_EDS_EPA_H_

#include "eds_object.h"

struct eds_object__epa;
struct eds_object__epn;

void
eds_epa__set_epn(struct eds_object__epa * epa, struct eds_object__epn * epn);

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_EPA_H_ */
