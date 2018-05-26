#ifndef _tasks_h_
#define _tasks_h_

#include "common.h"
//#include "includes.h"
#include "scheduler.h"


#define tasksSize 6
Task tasks[tasksSize];
Byte i = 0;
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