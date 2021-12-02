/*
 * mem_pool.h
 *
 *  Created on: Nov 7, 2021
 *      Author: Nenad.Radulovic
 */

#ifndef GENERIC_MEM_MEM_POOL_H_
#define GENERIC_MEM_MEM_POOL_H_

#define NK_MEM_POOL__ENTRY_T(type)                                          \
        {                                                                   \
            struct nk_mem_pool * pool;                                      \
            type entry;                                                     \
        }

#define NK_MEM_POOL__

#endif /* GENERIC_MEM_MEM_POOL_H_ */
