#ifndef _DISPLAY_H_
#define _DISPLAY_H_

//includes
#include "common.h"
#include "menu.h"

//defines
#define PATTERN_SIZE (NUM_ROWS)

//typedefs

typedef enum {
    eTEXT_FIRST_ENTRY,
    eTEXT_ES_IST = eTEXT_FIRST_ENTRY,
    eTEXT_NULL_H,
    eTEXT_NULL_UHR,
    eTEXT_FUENF,
    eTEXT_ZEHN,
    eTEXT_FUENFZEHN,
    eTEXT_VIERTEL,
    eTEXT_ZWANZIG,
    eTEXT_DREIVIERTEL,
    eTEXT_VOR,
    eTEXT_NACH,
    eTEXT_HALB,
    eTEXT_MITTERNACHT,
    eTEXT_ZWOELF_H,
    eTEXT_EINS_H,
    eTEXT_ZWEI_H,
    eTEXT_DREI_H,
    eTEXT_VIER_H,
    eTEXT_FUENF_H,
    eTEXT_SECHS_H,
    eTEXT_SIEBEN_H,
    eTEXT_ACHT_H,
    eTEXT_NEUN_H,
    eTEXT_ZEHN_H,
    eTEXT_ELF_H, //eZWOELF_H to eELF_H must stay in successive order
    eTEXT_EIN_H,
    eTEXT_UHR,
    eTEXT_NUM_ENTRIES
} TE_CLOCK_TEXT;

//declarations
void vWriteTime(TS_TIME *pstTime, TE_CONFIG eConfig);
void vTestDisplay(void);
void vClearPattern(void);
void vSetInPattern(U8 u8Col, U8 u8Row, U8 u8On);
void vAddNumToPattern(U8 u8Num, U8 u8Col, U8 u8Row);
void vWritePattern(void);
void vAddTextToPattern(TE_CLOCK_TEXT eText);

#endif
