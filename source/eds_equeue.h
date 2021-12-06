/*
 * eds_equeue.h
 *
 *  Created on: Dec 5, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_GENERIC_SOURCE_EDS_EQUEUE_H_
#define NEON_KIT_GENERIC_SOURCE_EDS_EQUEUE_H_

#include "eds_object.h"

#include <stddef.h>

size_t
eds_equeue__calculate_storage_size(size_t entries);

void
eds_equeue__init(struct eds_object__equeue *eq, size_t entries, struct eds_object__event *storage);

void
eds_equeue__term(struct eds_object__equeue *equeue);

void
eds_equeue__push(struct eds_object__equeue *equeue, const struct eds_object__event *event);

const struct eds_object__event*
eds_equeue__pop(struct eds_object__equeue *equeue);

bool
eds_equeue__is_empty(const struct eds_object__equeue *equeue);

#endif /* NEON_KIT_GENERIC_SOURCE_EDS_EQUEUE_H_ */
