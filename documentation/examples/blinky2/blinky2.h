#ifndef BLINKY2_H_
#define BLINKY2_H_

#include <stdint.h>
#include "eds.h"

struct blinky2_workspace
{
    eds__etimer * tick;
    uint32_t period_ms;
    uint32_t instance;
};

extern eds__sm_action blinky2_initial_state(eds__sm * sm, void * workspace, const eds__event * event);

#endif /* BLINKY2_H_ */
