/*
 * nk_compiler.h
 *
 *  Created on: May 8, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 *
 *  13/07/2021: (nbr) Added NK_COMPILER__NO_RETURN
 *  14/07/2021: (nbr) Added NK_COMPILER__IGNORE_ARG
 */
/**
 * @file
 * @brief   Neon Kit Compiler (PLATFORM)
 * @{
 */

#ifndef NEON_KIT_GENERIC_NK_COMPILER_H_
#define NEON_KIT_GENERIC_NK_COMPILER_H_

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @brief   Compiler directive to generate a function which does not return
 */
#define NK_COMPILER__NO_RETURN          __attribute__((noreturn))
    
    
#define NK_COMPILER__IGNORE_ARG(arg)    (void)(arg);

#define NK_COMPILER__CONTAINER_OF(ptr, type, member)                        \
        (type *)((void *)((char *)(ptr) - offsetof(type, member)))

#define NK_COMPILER__CONTAINER_OF_CONST(ptr, type, member)                  \
        (const type *)((const void *)((const char *)(ptr) - offsetof(type, member)))

/**
 * @brief   Forward declaration of string
 */
struct nk_string;

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