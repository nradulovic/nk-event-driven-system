/**
 * @file
 * @date        Dec 2, 2021
 * @brief       Nano-Kit Event Driven System (EDS) configuration header
 *
 * @author      Nenad Radulovic (nenad.b.radulovic@gmail.com)
 * @authors     Nenad Radulovic (nenad.b.radulovic@gmail.com)
 *
 * @defgroup    eds_config Configuration
 * @brief       Event Driven System (EDS) configuration
 *
 * This module contains macros for global compile-time EDS configuration.
 *
 * @{
 */

#ifndef NEON_KIT_EDS_SOURCE_EDS_CONFIG_H_
#define NEON_KIT_EDS_SOURCE_EDS_CONFIG_H_

#if defined(EDS_CONFIG__ENABLE)
#include "eds_config_definition.h"
#endif

/**
 * @defgroup    eds_config_sm State machine (SM)
 * @brief       State machine (SM) configuration.
 * @{
 */

/**
 * @brief       Enable or disable execution of hierarchical state machines (HSM).
 */
#ifndef EDS_CONFIG__SMP__ENABLE_HSM
#define EDS_CONFIG__SMP__ENABLE_HSM         1
#endif

/**
 * @brief       Define maximum number of hierarchical level in hierarchical state machines (HSM).
 */
#ifndef EDS_CONFIG__SMP__HSM_LEVELS
#define EDS_CONFIG__SMP__HSM_LEVELS         8
#endif

/** @} */
/**
 * @defgroup    eds_config_agent Event Processing Agent (EPA)
 * @brief       Event Processing Agent (EPA) configuration.
 * @{
 */

/**
 * @brief       Enable or disable agent instance name storing.
 */
#ifndef EDS_CONFIG__AGENT__ENABLE_NAME
#define EDS_CONFIG__AGENT__ENABLE_NAME      1
#endif

/** @} */
/**
 * @defgroup    eds_config_network Event Processing Network (EPN)
 * @brief       Event Processing Network (EPN) configuration.
 * @{
 */

/**
 * @brief       Enable or disable network callbacks.
 */
#ifndef EDS_CONFIG__NETWORK__ENABLE_CALLBACKS
#define EDS_CONFIG__NETWORK__ENABLE_CALLBACKS 1
#endif

/** @} */
#endif /* NEON_KIT_EDS_SOURCE_EDS_CONFIG_H_ */
/** @} */
