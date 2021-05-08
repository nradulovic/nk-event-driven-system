/*
 * nk_farray.h
 *
 *  Created on: Apr 13, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 *
 *  08/05/2021: (nbr) Added NK_FARRAY__SIZE() macro
 */

#ifndef GENERIC_NK_FARRAY_H_
#define GENERIC_NK_FARRAY_H_

#define NK_FARRAY__T(item_type, item_no)                                    \
		{                                                                   \
            item_type items[item_no];                                       \
		}

#define NK_FARRAY__LENGTH(self)                                             \
        (sizeof((self)->items) / sizeof((self)->items[0]))

#define NK_FARRAY__SIZE(self)                                               \
        NK_FARRAY__LENGTH(self)

#endif /* GENERIC_NK_FARRAY_H_ */
