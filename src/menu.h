#ifndef _MENU_H_
#define _MENU_H_

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
  eMENU_TIME_INVALID,
  eMENU_TOGGLE_SECOND
}TE_MENU_RC;

TE_MENU_RC eHandleButton(TE_BUTTONS eButton);
U24 u24MenuGetTime(void);

#endif
