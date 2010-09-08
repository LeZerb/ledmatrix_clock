#ifndef _DISPLAY_H_
#define _DISPLAY_H_

//includes
#include "common.h"

//defines
#define PATTERN_SIZE (((NUM_COLS * NUM_ROWS) / 8) + 1)

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
void vClearPattern(void);
void vSetInPattern(U8 u8Col, U8 u8Row, U8 u8On);
void vAddNumToPattern(U8 u8Num, U8 u8Col, U8 u8Row);
void vWritePattern(void);

#endif
