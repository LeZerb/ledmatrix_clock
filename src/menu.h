#ifndef _MENU_H_
#define _MENU_H_

#include "date.h"

//typedefs
typedef enum
{
  //do not change the button order
  eBUTTON_MENU,
  eBUTTON_SET,
  eBUTTON_COUNT
}TE_BUTTONS;

typedef enum
{
  eMENU_RC_OK,
  eMENU_RC_ENTERED,
  eMENU_RC_LEFT,
  eMENU_RC_TIME_AVAIL,
  eMENU_RC_DATE_AVAIL,
  eMENU_RC_TIME_INVALID,
  eMENU_RC_SHOW_SECOND,
  eMENU_RC_SHOW_DAY,
  eMENU_RC_SHOW_YEAR,
  eMENU_RC_CONFIG_CHANGED,
  eMENU_RC_ERROR
}TE_MENU_RC;

typedef enum
{
  eMENU_CONF_DREIVIERTEL = 0b0,
  eMENU_CONF_VIERTEL_VOR = 0b1,          
  eMENU_CONF_VIERTEL_VOR_BIT = eMENU_CONF_VIERTEL_VOR,         //bit 0 identifies viertel nach/vor mode
  eMENU_CONF_DREIVIERTEL_ZWANZIG = 0b10,
  eMENU_CONF_ZWANZIG_VOR_BIT = eMENU_CONF_DREIVIERTEL_ZWANZIG, //bit 1 identifies zwanzig nach/vor mode
  eMENU_CONF_VIERTEL_VOR_ZWANZIG = 0b11,
  eMENU_CONF_NUM
}TE_MENU_CONFIG;

//functions
TE_MENU_RC eHandleButton(TE_BUTTONS eButton, U24 u24CurTime, TS_DATE *pstCurDate, TE_MENU_CONFIG eConfig);
U24 u24MenuGetTime(void);
void vMenuGetDate(TS_DATE *pstDate);
TE_MENU_CONFIG eMenuGetConfig(void);

#endif
