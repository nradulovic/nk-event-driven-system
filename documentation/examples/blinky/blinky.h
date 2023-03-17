
#ifndef BLINKY_H_
#define BLINKY_H_

#include "eds.h"

struct blinky_workspace
{
    eds__etimer * tick;
};

extern eds__sm_action blinky_initial_state(eds__sm * sm, void * workspace, const eds__event * event);

#endif /* BLINKY_H_ */