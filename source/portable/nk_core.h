/*
 * nk_core.h
 *
 *  Created on: May 8, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 *  08/05/2021: (nbr) Initial CORE interface definition
 *  13/07/2021: (nbr) Added nk_core__reset
 *  14/07/2021: (nbr) Added nk_core__g__sys_freq
 */
/**
 * @file
 * @brief   Neon Kit Core (PSP)
 * @{
 */

#ifndef NEON_KIT_GENERIC_NK_CORE_H_
#define NEON_KIT_GENERIC_NK_CORE_H_

#include <stdint.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#include "generic/portable/nk_compiler.h"

extern uint32_t nk_core__g__sys_freq;
    
struct nk_tasker__array;

struct nk_tasker__array * nk_core__tasker_instances(void);

NK_COMPILER__NO_RETURN
void nk_core__reset(void);

uint32_t nk_core__instruction_count(void);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
#endif /* NEON_KIT_GENERIC_NK_CORE_H_ */
