/*
 * eds_port_definition.h
 *
 *  Created on: Dec 14, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_EDS_PORTABLE_GCC_ANY_ANY_FREERTOS_EDS_PORT_DEFINITION_H_
#define NEON_KIT_EDS_PORTABLE_GCC_ANY_ANY_FREERTOS_EDS_PORT_DEFINITION_H_

struct eds_port__sleep_local
{
    int dummy;
};

struct eds_port__critical_local
{
    int dummy;
};

struct eds_port__timer
{
    int dummy;
};

struct eds_port__atomic
{
    int counter;
};

int eds_port__atomic_test_and_set(struct eds_port__atomic * atomic);

#endif /* NEON_KIT_EDS_PORTABLE_GCC_ANY_ANY_FREERTOS_EDS_PORT_DEFINITION_H_ */
