#ifndef _MENU_H_
#define _MENU_H_

#include "date.h"

typedef enum
{
  eBUTTON_MENU,
  eBUTTON_SET
}TE_BUTTONS;

typedef enum
{
  eMENU_OK,
  eMENU_ENTERED,
  eMENU_LEFT,
  eMENU_TIME_AVAIL,
  eMENU_DATE_AVAIL,
  eMENU_TIME_INVALID,
  eMENU_SHOW_SECOND,
  eMENU_SHOW_DAY,
  eMENU_SHOW_YEAR
}TE_MENU_RC;

TE_MENU_RC eHandleButton(TE_BUTTONS eButton);
U24 u24MenuGetTime(void);
void vMenuGetDate(TS_DATE *pstDate);

#endif
