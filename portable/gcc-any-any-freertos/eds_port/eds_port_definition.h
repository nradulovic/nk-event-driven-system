/*
 * eds_port_definition.h
 *
 *  Created on: Dec 14, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_EDS_PORTABLE_GCC_ANY_ANY_FREERTOS_EDS_PORT_DEFINITION_H_
#define NEON_KIT_EDS_PORTABLE_GCC_ANY_ANY_FREERTOS_EDS_PORT_DEFINITION_H_

#if defined(ESP_IDF)
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#endif

#define EDS_PORT__USE_GLOBAL_CRITICAL

struct eds_object__tmr_sentinel;

struct eds_port__sleep
{
    SemaphoreHandle_t instance;
    StaticSemaphore_t buffer;
};

struct eds_port__critical
{
    SemaphoreHandle_t instance;
    StaticSemaphore_t buffer;
};


#endif /* NEON_KIT_EDS_PORTABLE_GCC_ANY_ANY_FREERTOS_EDS_PORT_DEFINITION_H_ */
