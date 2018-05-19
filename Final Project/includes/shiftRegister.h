#ifndef _shiftRegister_h_
#define _shiftRegister_h_

#include "bit.h"

/* NOTES 
 * H is least significant bit (Pin 8)
 * A is most significant bit (Pin 15)
 */

#define OUTPORT PORTB
#define SER 0		// input
#define RCLCK 1		// moves data to storage on positive edge
#define SRCLCK 2    // shifts input on positive edge
//#define SRCLR 3

const unsigned char LOW = 0;
const unsigned char HIGH = 1;
const unsigned char numBits = 8;

unsigned char out;

//void clearSR() {
//out = SetBit(out, SRCLR, LOW); // set low to clear
//OUTPORT = out;
////delay_ms(2);
//out = SetBit(out, SRCLR, HIGH); // set back to high
//OUTPORT = out;
//}

void shiftBitIn(unsigned char bit) {
	out = SetBit(out, SER, bit);			// write bit to latch
	out = SetBit(out, SRCLCK, HIGH);		// change state to high to save previous input
	OUTPORT = out;
	out = SetBit(out, SRCLCK, LOW);		// change back to low
	OUTPORT = out;
}

void shiftToOutput() {
	out = SetBit(out, RCLCK, HIGH);		// change high to output all registers
	OUTPORT = out;
	out = SetBit(out, RCLCK, LOW);		// change back to low
	OUTPORT = out;
}

// shift in n bits from data
void shiftDataIn(unsigned char data, unsigned char n) {
	for (unsigned char i = 0; i <= n; ++i) {
		shiftBitIn(GetBit(data,i));
	}
	shiftToOutput(); // shift data out
}

#endif