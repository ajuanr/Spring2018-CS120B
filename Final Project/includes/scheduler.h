#ifndef _scheduler_h_
#define _scheduler_h_

#include "common.h"

typedef struct Task {
    State state;
    unsigned long elapsedTime;
    unsigned long period;
    State (*tckFct)(State);
} Task;


// for use with the timerISR
Task *tasks;
Byte tasksSize;
unsigned long period;

// set the required variable for the timerISR
void timerISRSetup(Task *t, Byte size, unsigned long p) {
	tasks = t;
	tasksSize = size;
	period = p;
}



#endif