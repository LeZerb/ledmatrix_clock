#ifndef _DISPLAY_H_
#define _DISPLAY_H_

//includes
#include "common.h"
#include "menu.h"

//defines
#define PATTERN_SIZE (NUM_ROWS)

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
  eMITTERNACHT,
  eUHR,
  eNUM_ENTRIES
}TE_CLOCK_TEXT;

//declarations
void vWriteTime(TS_TIME *pstTime, TE_MENU_CONFIG eConfig);
void vTestDisplay(void);
void vClearPattern(void);
void vSetInPattern(U8 u8Col, U8 u8Row, U8 u8On);
void vAddNumToPattern(U8 u8Num, U8 u8Col, U8 u8Row);
void vWritePattern(void);
void vAddTextToPattern(TE_CLOCK_TEXT eText);

#endif
