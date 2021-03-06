#include "bit.h"

#define pin PINA 
// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
/* Keypad arrangement
        PC4 PC5 PC6 PC7
   col  1   2   3   4
row
PC0 1   1 | 2 | 3 | A
PC1 2   4 | 5 | 6 | B
PC2 3   7 | 8 | 9 | C
PC3 4   * | 0 | # | D
*/
unsigned char GetKeypadKey() {

    // 11101111
    PORTA = 0xEF; // Enable col 4 with 0, disable others with 1s
    asm("nop"); // add a delay to allow PORTA to stabilize before checking
    if (GetBit(pin,0)==0) { return('1'); }
    if (GetBit(pin,1)==0) { return('4'); }
    if (GetBit(pin,2)==0) { return('7'); }
    if (GetBit(pin,3)==0) { return('*'); }

    // Check keys in col 2
    // 11011111
    PORTA = 0xDF; // Enable col 5 with 0, disable others with 1s
    asm("nop"); // add a delay to allow PORTA to stabilize before checking
    if (GetBit(pin,0)==0) { return('2'); }
    if (GetBit(pin,1)==0) { return('5'); }
    if (GetBit(pin,2)==0) { return('8'); }
    if (GetBit(pin,3)==0) { return('0'); }

    // Check keys in col 3
    // 10111111
    PORTA = 0xBF; // Enable col 6 with 0, disable others with 1s
    asm("nop"); // add a delay to allow PORTA to stabilize before checking
    if (GetBit(pin,0)==0) { return('3'); }
    if (GetBit(pin,1)==0) { return('6'); }
    if (GetBit(pin,2)==0) { return('9'); }
    if (GetBit(pin,3)==0) { return('#'); }

    // Check keys in col 4  
    // 01111111
    PORTA = 0x7F; // Enable col 7 with 0, disable others with 1s
    asm("nop"); // add a delay to allow PORTA to stabilize before checking
    if (GetBit(pin,0)==0) { return('A'); }
    if (GetBit(pin,1)==0) { return('B'); }
    if (GetBit(pin,2)==0) { return('C'); }
    if (GetBit(pin,3)==0) { return('D'); }

    return('\0'); // default value
}