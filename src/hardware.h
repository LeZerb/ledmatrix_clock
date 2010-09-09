#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#define _XTAL_FREQ  (19660800)

//WRAPS_A_SEC defines how many timer overflows are in one minute
//this is dependant on clock frequency and timer prescaler
#define WRAPS_A_SEC       (75)

//which PINS are to be used as output
//columns
#define OUTPUT0  TRISA0
#define OUTPUT1  TRISA1
#define OUTPUT2  TRISA2
#define OUTPUT3  TRISA3
#define OUTPUT4  TRISA4
#define OUTPUT5  TRISA5
#define OUTPUT6  TRISE0
#define OUTPUT7  TRISE1
#define OUTPUT8  TRISE2
#define OUTPUT9  TRISC0
#define OUTPUT10 TRISC1
//rows
#define OUTPUT11 TRISB1
#define OUTPUT12 TRISB0
#define OUTPUT13 TRISD7
#define OUTPUT14 TRISD6
#define OUTPUT15 TRISD5
#define OUTPUT16 TRISD4
#define OUTPUT17 TRISC7
#define OUTPUT18 TRISC6
#define OUTPUT19 TRISC5
#define OUTPUT20 TRISC4
#define OUTPUT21 TRISD3
#define OUTPUT23 TRISD2
//DCF power
#define OUTPUT24 TRISC3

//LED matrix
#define NUM_COLS (11)
#define NUM_ROWS (12)

#define COL0  RA0
#define COL1  RA1
#define COL2  RA2
#define COL3  RA3
#define COL4  RA4
#define COL5  RA5
#define COL6  RE0
#define COL7  RE1
#define COL8  RE2
#define COL9  RC0
#define COL10 RC1

#define ROW0  RB1
#define ROW1  RB0
#define ROW2  RD7
#define ROW3  RD6
#define ROW4  RD5
#define ROW5  RD4
#define ROW6  RC7
#define ROW7  RC6
#define ROW8  RC5
#define ROW9  RC4
#define ROW10 RD3
#define ROW11 RD2

//DCF receiver
#define DCF_POWER    RC3
#define DCF_IN       RB2
#define DCF_INVERTED (1)

//switches
#define SW_MENU RB4
#define SW_SET  RB3

#endif