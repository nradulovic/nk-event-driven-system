/*
 * nk_error.h
 *
 *  Created on: Apr 11, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 *
 *  14/07/2021: (nbr) Added NK_ERROR__CONFIG_INVALID
 */

#ifndef NEON_KIT_GENERIC_NK_ERROR_H_
#define NEON_KIT_GENERIC_NK_ERROR_H_

#if defined(__cplusplus)
extern "C"
{
#endif

enum nk_error
{
    NK_ERROR__OK,
    /* Query and search errors */
    NK_ERROR__NOT_FOUND,

    /* Data error */
    NK_ERROR__DATA_INVALID,
    NK_ERROR__DATA_ODD,
    NK_ERROR__DATA_OVF,
    NK_ERROR__DATA_UNDERFLOW,
    
    /* Configuration error */
    NK_ERROR__CONFIG_INVALID,

    /* Buffer error */
    NK_ERROR__BUFFER_OVF
};

#if defined(__cplusplus)
}
#endif

#endif /* NEON_KIT_GENERIC_NK_ERROR_H_ */
