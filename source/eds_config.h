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
 * @brief       Enable State Machine Processor (SMP) hierarchical state machines (HSM) support.
 *
 *              Enable or disable execution of hierarchical state machines (HSM) in the SMP module.
 *
 *              If this option is disabled only non-hierarchical state machines, also known as
 *              Finite State Machine (FSM), may be executed.
 */
#ifndef EDS_CONFIG__SMP__ENABLE_HSM
#define EDS_CONFIG__SMP__ENABLE_HSM         1
#endif

/**
 * @brief       Maximum hierarchical state machines (HSM) state nesting levels.
 *
 *              Define the maximum number of state nesting levels in hierarchical state machines
 *              (HSM).
 *
 *              This option is ignored when @ref EDS_CONFIG__SMP__ENABLE_HSM is not enabled (set to
 *              zero).
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
 * @brief       Enable Event Processing Agent (EPA) name support.
 *
 *              Enable or disable the EPA instance to store reference to a constant string
 *              containing the name.
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
 * @brief       Enable Event Processing Network (EPN) name support.
 *
 *              Enable or disable the EPN instance to store reference to a constant string
 *              containing the name.
 */
#ifndef EDS_CONFIG__NETWORK__ENABLE_NAME
#define EDS_CONFIG__NETWORK__ENABLE_NAME    1
#endif

/**
 * @brief       Enable Event Processing Network (EPN) callbacks.
 *
 *              EPN callbacks are called on the following events:
 *              - when network wants to switch from idle state to run state.
 *              - when network wants to switch from run state to idle state,
 *
 *              The callbacks are passed to an EPN at the moment of the EPN instance creation.
 */
#ifndef EDS_CONFIG__NETWORK__ENABLE_CALLBACKS
#define EDS_CONFIG__NETWORK__ENABLE_CALLBACKS 1
#endif

/** @} */
#endif /* NEON_KIT_EDS_SOURCE_EDS_CONFIG_H_ */
/** @} */
