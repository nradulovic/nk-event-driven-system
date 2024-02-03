/*
 * blinky.c
 *
 *  Created on: Dec 14, 2021
 *      Author: nenad
 */

#include <assert.h>
#include "eds.h"
#include "std_allocator.h"
#include "blinky.h"

int main(void)
{
    eds__error error;
    eds__agent * sm_blink_agent;
    eds__network * network;
    struct sm__workspace sm__blink_workspace;

    error = eds__mem_add_allocator(std_allocator_alloc, std_allocator_free, NULL, STD_ALLOCATOR_MAX_SIZE);
	assert(error == EDS__ERROR_NONE);

    error = eds__agent_create(blinky_initial_state, &sm__blink_workspace, NULL, &sm_blink_agent);
    assert(error == EDS__ERROR_NONE);

    error = eds__network_create(NULL, &network);
    assert(error == EDS__ERROR_NONE);

    error = eds__network_add_agent(network, sm_blink_agent);
    assert(error == EDS__ERROR_NONE);

    error = eds__network_start(network);
    assert(error == EDS__ERROR_NONE);

    return 0;
}
