#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_

//defines
#define DEBUG 0

#define SECS_IN_MIN              (60)
#define SECS_IN_5_MIN (5*SECS_IN_MIN)
#define SECS_IN_HOUR (60*SECS_IN_MIN)
#define SECS_A_DAY  (24*SECS_IN_HOUR)

#define NUM_COLS 11
#define NUM_ROWS 12

#define _XTAL_FREQ  19660800
#define WRAPS_A_SEC       75
#define ITR_RESET_AFTER    3

#define DELAY_MS(x) CLRWDT(); __delay_ms((x)); CLRWDT();
#define DELAY_US(x) CLRWDT(); __delay_us((x)); CLRWDT();

#define BIT_SET(var, bitno) ((var) |= 1UL << (bitno))
#define BIT_CLR(var, bitno) ((var) &= ~(1UL << (bitno)))

#endif
