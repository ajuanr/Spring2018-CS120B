#ifndef scheduler_h
#define scheduler_h

#include "common.h"
#include "motion_detector.h"

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


void timerISR() {
	for (Byte i = 0; i != tasksSize; ++i) {
				if (!motionSensed &&  tasks[i].tckFct != &motionTckFct) {
					; // no motion detected, do nothing until motion resumes
				}
				else {
					if (tasks[i].elapsedTime > tasks[i].period) {
						tasks[i].state = tasks[i].tckFct(tasks[i].state);
						tasks[i].elapsedTime = 0;
					}
					else {
						tasks[i].elapsedTime += period;
					}
				}
	}
}

#endif