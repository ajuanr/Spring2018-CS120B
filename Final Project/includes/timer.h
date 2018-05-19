#include <avr/interrupt.h>
#include "scheduler.h"

volatile unsigned char timerFlag = 0; // TimerISR sets to zero. Programer sets to 0;

unsigned long avrTimerM = 1; // start counting from here, down to 0. Default 1 ms
unsigned long avrTimer_currCnt = 0; // current internal count 1ms ticks

void TimerOn() {
	// AVR timer controller register TCCR1
	TCCR1B = 0x0B;
	
	// AVR output compare register OCR1A
	OCR1A = 125;
	
	// AVR timer interrupt mask register
	TIMSK1 = 0x02;
	
	// initialize AVR counter
	TCNT1 = 0;
	
	avrTimer_currCnt = avrTimerM;
	// timer will be called every avrTime_currCnt milliseconds
	
	// Enable global interrupts
	SREG |= 0x80;
}

void timerOff() {
	TCCR1B = 0x00;
}

//void timerISR() {
	//timerFlag = 1;
//}

ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls TCNT1 == OCR1 (every 1 ms per timerOn() settings
	avrTimer_currCnt--;
	if (avrTimer_currCnt == 0) {
		timerISR(); // call the ISR that the user uses
		avrTimer_currCnt = avrTimerM;
	}
}

// set timerISR to tick every M milliseconds
void timerSet(unsigned long M) {
	avrTimerM = M;
	avrTimer_currCnt = avrTimerM;
}