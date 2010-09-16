#ifndef _COMMON_H_
#define _COMMON_H_

//common includes
#include <htc.h>
#include <string.h>
#include "hardware.h"

#define U8  unsigned char
#define U16 unsigned short int
#define U24 unsigned short long int
#define U32 unsigned long int

#define SECS_IN_MIN                        (60)
#define SECS_IN_5_MIN         (5 * SECS_IN_MIN)

#define DELAY_MS(x) CLRWDT(); __delay_ms((x));
#define DELAY_US(x) CLRWDT(); __delay_us((x));

#define BIT_SET_8(var, bitno) ((var) |=   (U8)1 << (bitno))
#define BIT_CLR_8(var, bitno) ((var) &= ~((U8)1 << (bitno)))

#endif
