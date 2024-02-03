/*
 * eds_port_definition.h
 *
 *  Created on: Dec 14, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_EDS_DOCUMENTATION_EXAMPLES_BLINKY_EDS_PORT_EDS_PORT_DEFINITION_H_
#define NEON_KIT_EDS_DOCUMENTATION_EXAMPLES_BLINKY_EDS_PORT_EDS_PORT_DEFINITION_H_

#include <semaphore.h>
#include <pthread.h>

#define EDS_PORT__USE_GLOBAL_CRITICAL

struct eds_port__sleep
{
    sem_t semaphore;
};

struct eds_port__critical
{
	pthread_mutex_t mutex;
};

#endif /* NEON_KIT_EDS_DOCUMENTATION_EXAMPLES_BLINKY_EDS_PORT_EDS_PORT_DEFINITION_H_ */
