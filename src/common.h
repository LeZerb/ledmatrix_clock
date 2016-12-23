#ifndef _COMMON_H_
#define _COMMON_H_

//common includes
#include <xc.h>
#include <string.h>
#include <stdint.h>
#include "hardware.h"

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;

typedef bit BOOL;
#define FALSE (0)
#define TRUE  (1)

#define SECS_IN_MIN                        (60)
#define SECS_IN_5_MIN         (5 * SECS_IN_MIN)

#define DELAY_MS(x) __delay_ms((x))
#define DELAY_US(x) __delay_us((x))

#define BIT_SET_8(var, bitno) ((var) |=   (U8)1 << (bitno))
#define BIT_CLR_8(var, bitno) ((var) &= ~((U8)1 << (bitno)))

#endif
