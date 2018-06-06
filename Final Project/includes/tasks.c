#ifndef _tasks_h
#define _tasks_h

#include "scheduler.h"
#include "includes.h"

ConstByte period = 25;
ConstByte tasksSize = 7;
Task tasks[7];
Byte i = 0;
tasks[i].state = 0;
tasks[i].elapsedTime = 0;
tasks[i].period = playPeriod;
tasks[i].tckFct = &playTckFct;
++i;
tasks[i].state = 0;
tasks[i].elapsedTime = 0;
tasks[i].period = joyStickPeriod;
tasks[i].tckFct = &joystickTckFct;
++i;
tasks[i].state = 0;
tasks[i].elapsedTime = 0;
tasks[i].period = buttonPeriod;
tasks[i].tckFct = &buttonTckFct;
++i;
tasks[i].state = 0;
tasks[i].elapsedTime = 0;
tasks[i].period = POS_PERIOD;
tasks[i].tckFct = &posTckFct;
++i;
tasks[i].state = 0;
tasks[i].elapsedTime = 0;
tasks[i].period = JUMP_PERIOD;
tasks[i].tckFct = &jumpTckFct;
++i;
tasks[i].state = 0;
tasks[i].elapsedTime = 0;
tasks[i].period = PROJ_PERIOD;
tasks[i].tckFct = &projTckFct;
++i;
tasks[i].state = 0;
tasks[i].elapsedTime = 0;
tasks[i].period = RENDER_PERIOD;
tasks[i].tckFct = &LCDtckFct;

#endif