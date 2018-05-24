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

#define LOW 0
#define HIGH 1
#define numBits 8

unsigned char SRout ;

//void clearSR() {
//SRout  = SetBit(SRout , SRCLR, LOW); // set low to clear
//OUTPORT = SRout ;
////delay_ms(2);
//SRout  = SetBit(SRout , SRCLR, HIGH); // set back to high
//OUTPORT = SRout ;
//}

void shiftBitIn(const unsigned char bit) {
	SRout  = SetBit(SRout , SER, bit);			// write bit to latch
	SRout  = SetBit(SRout , SRCLCK, HIGH);		// change state to high to save previous input
	OUTPORT = SRout ;
	SRout  = SetBit(SRout , SRCLCK, LOW);		// change back to low
	OUTPORT = SRout ;
}

void shiftToOutput() {
	SRout  = SetBit(SRout , RCLCK, HIGH);		// change high to output all registers
	OUTPORT = SRout ;
	SRout  = SetBit(SRout , RCLCK, LOW);		// change back to low
	OUTPORT = SRout ;
}

// shift in n bits from data
void shiftDataIn(const unsigned char data) {
	for (unsigned char i = 0; i <= numBits; ++i) {
		shiftBitIn(GetBit(data,i));
		shiftToOutput(); // shift data out
	}
	
}

#endif