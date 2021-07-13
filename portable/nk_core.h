/*
 * nk_core.h
 *
 *  Created on: May 8, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 *  08/05/2021: (nbr) Initial CORE interface definition
 *  13/07/2021: (nbr) Added nk_core__reset
 */
/**
 * @file
 * @brief   Neon Kit Core (PSP)
 * @{
 */

#ifndef NEON_KIT_GENERIC_NK_CORE_H_
#define NEON_KIT_GENERIC_NK_CORE_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include "generic/portable/nk_compiler.h"

struct nk_tasker__array;

struct nk_tasker__array * nk_core__tasker_instances(void);

NK_COMPILER__NO_RETURN
void nk_core__reset(void);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
#endif /* NEON_KIT_GENERIC_NK_CORE_H_ */
