#ifndef BLINKY3_H_
#define BLINKY3_H_

#include "eds.h"

struct blinky3_workspace
{
    eds__etimer * tick;
    uint32_t period_ms;
    uint32_t instance;
};

extern eds__sm_action blinky3_initial_state(eds__sm * sm, void * workspace, const eds__event * event);

#endif /* BLINKY3_H_ */