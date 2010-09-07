#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "ledmatrix.h"

//defines
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

//typedefs
typedef enum
{
  eFIRST_ENTRY,
  eES_IST = eFIRST_ENTRY,
  eFUENF,
  eZEHN,
  eVIERTEL,
  eZWANZIG,
  eDREIVIERTEL,
  eGENAU,
  eVOR,
  eNACH,
  eHALB,
  eZWOELF,
  eEINS,
  eZWEI,
  eDREI,
  eVIER,
  eFUENF_H,
  eSECHS,
  eSIEBEN,
  eACHT,
  eNEUN,
  eZEHN_H,
  eELF,
  eEIN,
  eSEX,
  eMITTERNACHT,
  eUHR,
  eNUM_ENTRIES
}TE_CLOCK_TEXT;

typedef struct
{
  const unsigned char ucRow;
  const unsigned char aucCols[NUM_COLS / 8 + 1];
}TS_POSITION;

//declarations
void vWriteTime(unsigned short long u24TimeInSecs, unsigned char u8Ossi);
void vTestDisplay(void);
void vWritePattern(unsigned char *pu8Pattern);

#endif
