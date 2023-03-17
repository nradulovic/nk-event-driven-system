/*
 * eds_state.h
 *
 *  Created on: Jun 15, 2022
 *      Author: nenad
 */

#ifndef SOURCE_SYS_EDS_STATE_H_
#define SOURCE_SYS_EDS_STATE_H_

#include "eds_object.h"

#include <stdbool.h>

extern struct eds_object__vector eds_state__mem_instances;
extern struct eds_object__list eds_state__epn_instances;
extern struct eds_object__tmr_sentinel eds_state__tmr_sentinel;
extern bool eds_state__has_started;
extern bool eds_state__is_port_initialized;

#endif /* SOURCE_SYS_EDS_STATE_H_ */
