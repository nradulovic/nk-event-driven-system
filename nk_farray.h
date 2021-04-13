/*
 * nk_farray.h
 *
 *  Created on: Apr 13, 2021
 *      Author: nenad
 */

#ifndef GENERIC_NK_FARRAY_H_
#define GENERIC_NK_FARRAY_H_

#define NK_FARRAY__T(item_type, item_no)                                    \
		{                                                                   \
            item_type array[item_no];                                       \
		}

#define NK_FARRAY__INITIALIZE_WITH(self, ...)                               \
        {                                                                   \
            .array =                                                        \
            {                                                               \
                __VA_ARGS__                                                 \
            },                                                              \
        }

#endif /* GENERIC_NK_FARRAY_H_ */
