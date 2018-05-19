#ifndef scheduler_h
#define scheduler_h

#include "common.h"

typedef struct Task {
    State state;
    unsigned long elapsedTime;
    unsigned long period;
    State (*tckFct)(State);
} Task;

#endif