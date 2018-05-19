#ifndef scheduler_h
#define scheduler_h

typedef int State;

typedef struct Task {
    State state;
    unsigned long elapsedTime;
    unsigned long period;
    State (*tckFct)(State);
} Task;

#endif