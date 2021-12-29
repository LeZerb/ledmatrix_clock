#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include "xc.h"

#ifdef __16F887__
#elif defined __18F45K22__
#error _18F45K22
#include "mcc.h"
#else
#error Unsupported device
#endif

#define _XTAL_FREQ  (19660800ul)

//WRAPS_A_SEC defines how many timer overflows are in one minute
//this is dependant on clock frequency and timer prescaler
#define WRAPS_A_SEC       (75u)

//LED matrix
#define NUM_COLS (11)
#define NUM_ROWS (12) //last row is for 4 minute LEDs

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
