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
		if (tasks[i].elapsedTime > tasks[i].period) {
			tasks[i].state = tasks[i].tckFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		else {
			tasks[i].elapsedTime += period;
		}
	}
	// reset all states when reset is pressed
	if (gameReset) {
		for (Byte i = 0; i != tasksSize; ++i) {
			tasks[i].state = 0;
			tasks[i].elapsedTime = 0;
		}
		gameReset = false;
		gameOver = false;
	}
}

#endif