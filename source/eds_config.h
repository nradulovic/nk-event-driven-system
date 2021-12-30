/*
 * eds_config.h
 *
 *  Created on: Dec 29, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_EDS_SOURCE_EDS_CONFIG_H_
#define NEON_KIT_EDS_SOURCE_EDS_CONFIG_H_

#if defined(EDS_CONFIG__ENABLE)
#include "eds_config_definition.h"
#endif

#ifndef EDS_CONFIG__SMP__ENABLE_HSM
#define EDS_CONFIG__SMP__ENABLE_HSM         1
#endif

#ifndef EDS_CONFIG__SMP__HSM_LEVELS
#define EDS_CONFIG__SMP__HSM_LEVELS         8
#endif

#ifndef EDS_CONFIG__AGENT__ENABLE_NAME
#define EDS_CONFIG__AGENT__ENABLE_NAME      1
#endif

#ifndef EDS_CONFIG__NETWORK__ENABLE_NAME
#define EDS_CONFIG__NETWORK__ENABLE_NAME    1
#endif

#endif /* NEON_KIT_EDS_SOURCE_EDS_CONFIG_H_ */
