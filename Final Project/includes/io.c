#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"
#include "shiftRegister.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))
          
/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTC		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {
   LCD_WriteCommand(0x01);
}

void LCD_Write8(unsigned char);

void LCD_init(void) {

    //wait for 100 ms.
	delay_ms(100);
	
	LCD_Write8(0x30);
	delay_ms(10);
	LCD_Write8(0x30);
	delay_ms(10);
	LCD_Write8(0x30);
	delay_ms(10);
	LCD_Write8(0x20);			// in 4-bit mode now
	delay_ms(10);
	
	LCD_WriteCommand(0x28);		// 2 lines
	LCD_WriteCommand(0x08);	
	LCD_WriteCommand(0x01);		// clear display
	delay_ms(4);
	LCD_WriteCommand(0x06);	
	LCD_WriteCommand(0x0C);		// last 4 bits 1 D C B
										// display cursor blink: 1=on
	delay_ms(10);						 
}

// uses this before setting 4-bit mode or if using 8-bit mode
void LCD_Write8(unsigned char Command) {
	CLR_BIT(CONTROL_BUS,RS);
	DATA_BUS = Command;
	//shiftDataIn(Command, numBits);
	SET_BIT(CONTROL_BUS,E);
	asm("nop");
	CLR_BIT(CONTROL_BUS,E);
	delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteCommand (unsigned char Command) {
   CLR_BIT(CONTROL_BUS,RS);
   DATA_BUS = Command & 0xF0;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
      DATA_BUS = (Command << 4) & 0xF0;
      SET_BIT(CONTROL_BUS,E);
      asm("nop");
      CLR_BIT(CONTROL_BUS,E);
   delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
   SET_BIT(CONTROL_BUS,RS);
   DATA_BUS = Data & 0xF0;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(1);
      SET_BIT(CONTROL_BUS,RS);
      DATA_BUS = (Data << 4) & 0xF0;
      SET_BIT(CONTROL_BUS,E);
      asm("nop");
      CLR_BIT(CONTROL_BUS,E);
      delay_ms(1);
}

void LCD_customChar(unsigned char pos, unsigned char *data) {
	LCD_WriteCommand(0x40 + pos * 8);
	for (int i = 0; i != 8; ++i) {
		LCD_WriteData(data[i]);
	}
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
   LCD_ClearScreen();
   unsigned char c = column;
   while(*string) {
      LCD_Cursor(c++);
      LCD_WriteData(*string++);
   }
}

void LCD_Cursor(unsigned char column) {
   if ( column < 17 ) { // 16x1 LCD: column < 9
						// 16x2 LCD: column < 17
      LCD_WriteCommand(0x80 + column - 1);
   } else {
      LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
											// 16x2 LCD: column - 9
   }
}

void delay_ms(int miliSec) //for 8 Mhz crystal

{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
  {
   asm("nop");
  }
}