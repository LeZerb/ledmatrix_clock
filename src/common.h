#ifndef _COMMON_H_
#define _COMMON_H_

//common includes
#include <xc.h>
#include <string.h>
#include <stdint.h>
#include "hardware.h"

#define U8  uint8_t
#define U16 uint16_t
#define U32 uint32_t

#define SECS_IN_MIN                        (60)
#define SECS_IN_5_MIN         (5 * SECS_IN_MIN)

#define DELAY_MS(x) CLRWDT(); __delay_ms((x));
#define DELAY_US(x) CLRWDT(); __delay_us((x));

#define BIT_SET_8(var, bitno) ((var) |=   (U8)1 << (bitno))
#define BIT_CLR_8(var, bitno) ((var) &= ~((U8)1 << (bitno)))

#endif
