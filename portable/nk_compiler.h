/*
 * nk_compiler.h
 *
 *  Created on: May 8, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 *
 *  13/07/2021: (nbr) Added NK_COMPILER__NO_RETURN
 */
/**
 * @file
 * @brief   Neon Kit Compiler (PLATFORM)
 * @{
 */

#ifndef NEON_KIT_GENERIC_NK_COMPILER_H_
#define NEON_KIT_GENERIC_NK_COMPILER_H_

#include <stdint.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @brief   Compiler directive to generate a function which does not return
 */
#define NK_COMPILER__NO_RETURN          __attribute__((noreturn))

/**
 * @brief   Forward declaration of string
 */
struct nk_string;

uint32_t nk_compiler__instruction_count(void);

/**
 * @brief   Get compiler id string
 */
void nk_compiler__id(struct nk_string * id);

/**
 * @brief   Get compiler version string
 */
void nk_compiler__version(struct nk_string * version);

/**
 * @brief   Get compiler date string
 */
void nk_compiler__date(struct nk_string * date);

/**
 * @brief   Get compiler time string
 */
void nk_compiler__time(struct nk_string * time);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
#endif /* NEON_KIT_GENERIC_NK_COMPILER_H_ */
