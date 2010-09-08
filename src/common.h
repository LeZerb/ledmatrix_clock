#ifndef _COMMON_H_
#define _COMMON_H_

//common includes
#include <htc.h>
#include <string.h>
#include "hardware.h"

//defines
#define DEBUG_INPUTS  0
#define DEBUG_DCF     1
#define DEBUG_DISPLAY 2

//#define DEBUG DEBUG_INPUTS

#define U8  unsigned char
#define U16 unsigned short int
#define U24 unsigned short long int
#define U32 unsigned long int

#define SECS_IN_MIN                        (60)
#define SECS_IN_5_MIN         (5 * SECS_IN_MIN)
#define SECS_IN_HOUR         (60 * SECS_IN_MIN)
#define SECS_A_DAY    ((U24) 24 * SECS_IN_HOUR)

#define DELAY_MS(x) CLRWDT(); __delay_ms((x)); CLRWDT();
#define DELAY_US(x) CLRWDT(); __delay_us((x)); CLRWDT();

#define BIT_SET(var, bitno) ((var) |= 1UL << (bitno))
#define BIT_CLR(var, bitno) ((var) &= ~(1UL << (bitno)))

#endif
