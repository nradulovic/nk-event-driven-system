/*
 * nk_eds_port.h
 *
 *  Created on: Dec 2, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_NK_EDS_PORT_H_
#define NEON_KIT_GENERIC_SOURCE_NK_EDS_PORT_H_

#include <stddef.h>

#include "eds_port/eds_port_definition.h"

struct eds_port__sleep;

void eds_port__sleep__init(struct eds_port__sleep * sleep);
void eds_port__sleep__wait(struct eds_port__sleep * sleep);
void eds_port__sleep__signal(struct eds_port__sleep * sleep);

struct eds_port__critical;

void eds_port__critical__init(struct eds_port__critical * critical);
void eds_port__critical__lock(struct eds_port__critical * critical);
void eds_port__critical__unlock(struct eds_port__critical * critical);

size_t eds_port__align_up(size_t non_aligned_value);

#endif /* NEON_KIT_GENERIC_SOURCE_NK_EDS_PORT_H_ */
