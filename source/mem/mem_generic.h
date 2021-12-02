/*
 * mem_generic.h
 *
 *  Created on: Nov 7, 2021
 *      Author: Nenad.Radulovic
 */

#ifndef GENERIC_MEM_MEM_GENERIC_H_
#define GENERIC_MEM_MEM_GENERIC_H_

struct nk_mem
{
    void * (*alloc)(void);
    void (*dealloc)(void *);
};

#endif /* GENERIC_MEM_MEM_GENERIC_H_ */
