#ifndef _MENU_H_
#define _MENU_H_

#include "date.h"

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
  eMENU_RC_ERROR
}TE_MENU_RC;

TE_MENU_RC eHandleButton(TE_BUTTONS eButton, U24 u24CurTime, TS_DATE *pstCurDate);
U24 u24MenuGetTime(void);
void vMenuGetDate(TS_DATE *pstDate);

#endif
