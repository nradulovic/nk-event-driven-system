/*
 * blinky.c
 *
 *  Created on: Dec 14, 2021
 *      Author: nenad
 */

#include <assert.h>
#include "eds.h"
#include "std_allocator.h"
#include "blinky3.h"

int main(void)
{
    eds__error error;
    eds__agent * sm_blink_agent1;
    eds__agent * sm_blink_agent2;
    eds__network * network;
    struct blinky3_workspace sm__blink_workspace1 =
    {
        .period_ms = 1000, 
        .instance = 0
    };
    struct blinky3_workspace sm__blink_workspace2 =
    {
        .period_ms = 500, 
        .instance = 1
    };

    error = eds__mem_add_allocator(std_allocator_alloc, std_allocator_free, NULL, STD_ALLOCATOR_MAX_SIZE);
	assert(error == EDS__ERROR_NONE);

    error = eds__agent_create(blinky3_initial_state, &sm__blink_workspace1, NULL, &sm_blink_agent1);
    assert(error == EDS__ERROR_NONE);

    error = eds__agent_create(blinky3_initial_state, &sm__blink_workspace2, NULL, &sm_blink_agent2);
    assert(error == EDS__ERROR_NONE);

    error = eds__network_create(NULL, &network);
    assert(error == EDS__ERROR_NONE);

    error = eds__network_add_agent(network, sm_blink_agent1);
    assert(error == EDS__ERROR_NONE);

    error = eds__network_add_agent(network, sm_blink_agent2);
    assert(error == EDS__ERROR_NONE);

    error = eds__network_start(network);
    assert(error == EDS__ERROR_NONE);

    return 0;
}
