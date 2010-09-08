#include "common.h"
#include "display.h"
#include "menu.h"

//typedefs
typedef enum
{
  eMENU_ITEM_NIRVANA,
  eMENU_ITEM_TIME,
  eMENU_ITEM_HOUR,
  eMENU_ITEM_MINUTE,
  eMENU_ITEM_DATE,
  eMENU_ITEM_DAY,
  eMENU_ITEM_MONTH,
  eMENU_ITEM_YEAR,
  eMENU_ITEM_INVALIDATE_TIME,
  eMENU_ITEM_TEST_DISPLAY
}
TE_MENU_STATE;

static TE_MENU_STATE _eCurMenuItem = eMENU_ITEM_NIRVANA;

static U8 _u8Hour = 0, _u8Minute = 0, _u8Day = 1, _u8Month = 1, _u8Year = 10;

TE_MENU_RC eHandleButton(TE_BUTTONS eButton)
{
  TE_MENU_RC eRc = eMENU_OK;

  if (eButton == eBUTTON_MENU)
  {
    if (_eCurMenuItem == eMENU_ITEM_NIRVANA)
    {      
      //enter the initial menu state
      _eCurMenuItem = eMENU_ITEM_TIME;
      vClearPattern();

      vSetInPattern(0, 0, 1);
      
      eRc = eMENU_ENTERED;
    }
    else if (_eCurMenuItem == eMENU_ITEM_TIME)
    {
      //advance to next menu item (date)
      _eCurMenuItem = eMENU_ITEM_DATE;
      vClearPattern();

      vSetInPattern(0, 1, 1);
    }
    else if (_eCurMenuItem == eMENU_ITEM_DATE)
    {
      //advance to next menu item (display test)
      _eCurMenuItem = eMENU_ITEM_INVALIDATE_TIME;
      vClearPattern();

      vSetInPattern(0, 9, 1);
    }
    else if (_eCurMenuItem == eMENU_ITEM_INVALIDATE_TIME)
    {
      //advance to next menu item (display test)
      _eCurMenuItem = eMENU_ITEM_TEST_DISPLAY;
      vClearPattern();

      vSetInPattern(0, 10, 1);
    }
    else if (_eCurMenuItem == eMENU_ITEM_TEST_DISPLAY)
    {
      //we are now leaving the menu
      _eCurMenuItem = eMENU_ITEM_NIRVANA;

      vClearPattern();

      eRc = eMENU_LEFT;
    }
    else if (_eCurMenuItem == eMENU_ITEM_HOUR)
    {
      //increment hour
      _u8Hour++;

      if (_u8Hour == 24)
      {
        _u8Hour = 0;
      }

      vAddNumToPattern(_u8Hour / 10, 2, 3);
      vAddNumToPattern(_u8Hour % 10, 6, 3);
    }
    else if (_eCurMenuItem == eMENU_ITEM_MINUTE)
    {
      //increment minute
      _u8Minute++;

      if (_u8Minute == 60)
      {
        _u8Minute = 0;
      }

      vAddNumToPattern(_u8Minute / 10, 2, 3);
      vAddNumToPattern(_u8Minute % 10, 6, 3);
    }
    else if (_eCurMenuItem == eMENU_ITEM_DAY)
    {
      //increment day
      _u8Day++;

      if (_u8Day == 32)
      {
        _u8Day = 1;
      }

      vAddNumToPattern(_u8Day / 10, 2, 3);
      vAddNumToPattern(_u8Day % 10, 6, 3);
    }
    else if (_eCurMenuItem == eMENU_ITEM_MONTH)
    {
      //increment month
      _u8Month++;

      if (_u8Month == 13)
      {
        _u8Month = 1;
      }

      vAddNumToPattern(_u8Month / 10, 2, 3);
      vAddNumToPattern(_u8Month % 10, 6, 3);
    }
    else if (_eCurMenuItem == eMENU_ITEM_YEAR)
    {
      //increment year
      _u8Year++;

      if (_u8Year == 100)
      {
        _u8Year = 10;
      }

      vAddNumToPattern(_u8Year / 10, 2, 3);
      vAddNumToPattern(_u8Year % 10, 6, 3);
    }
  }
  else //button set
  {
    if (_eCurMenuItem == eMENU_ITEM_NIRVANA)
    {
      //toggle display of current second of minute
      eRc = eMENU_TOGGLE_SECOND;
    }
    if (_eCurMenuItem == eMENU_ITEM_TIME)
    {
      //enter submenu for setting the hour
      _eCurMenuItem = eMENU_ITEM_HOUR;

      vSetInPattern(1, 0, 1);
      vAddNumToPattern(_u8Hour / 10, 2, 3);
      vAddNumToPattern(_u8Hour % 10, 6, 3);
    }
    else if (_eCurMenuItem == eMENU_ITEM_HOUR)
    {
      //enter submenu for setting the minute
      _eCurMenuItem = eMENU_ITEM_MINUTE;

      vSetInPattern(2, 0, 1);
      vAddNumToPattern(_u8Minute / 10, 2, 3);
      vAddNumToPattern(_u8Minute % 10, 6, 3);
    }
    else if (_eCurMenuItem == eMENU_ITEM_MINUTE)
    {
      //go back to main menu
      _eCurMenuItem = eMENU_ITEM_TIME;
      vClearPattern();

      vSetInPattern(0, 0, 1);

      eRc = eMENU_TIME_AVAIL;
    }
    else if (_eCurMenuItem == eMENU_ITEM_DATE)
    {
      //enter submenu for setting the day
      _eCurMenuItem = eMENU_ITEM_DAY;

      vSetInPattern(1, 1, 1);
      vAddNumToPattern(_u8Day / 10, 2, 3);
      vAddNumToPattern(_u8Day % 10, 6, 3);
    }
    else if (_eCurMenuItem == eMENU_ITEM_DAY)
    {
      //enter submenu for setting the month
      _eCurMenuItem = eMENU_ITEM_MONTH;

      vSetInPattern(2, 1, 1);
      vAddNumToPattern(_u8Month / 10, 2, 3);
      vAddNumToPattern(_u8Month % 10, 6, 3);
    }
    else if (_eCurMenuItem == eMENU_ITEM_MONTH)
    {
      //enter submenu for setting the year
      _eCurMenuItem = eMENU_ITEM_YEAR;

      vSetInPattern(3, 1, 1);
      vAddNumToPattern(_u8Year / 10, 2, 3);
      vAddNumToPattern(_u8Year % 10, 6, 3);
    }
    else if (_eCurMenuItem == eMENU_ITEM_YEAR)
    {
      //go back to main menu
      _eCurMenuItem = eMENU_ITEM_DATE;
      vClearPattern();

      vSetInPattern(1, 0, 1);
    }
    else if (_eCurMenuItem == eMENU_ITEM_INVALIDATE_TIME)
    {
      //invalidate current time and stay on current item
      eRc = eMENU_TIME_INVALID;
    }
    else if (_eCurMenuItem == eMENU_ITEM_TEST_DISPLAY)
    {
      //perform the display test and stay on current item
      vTestDisplay();
    }
  }

  return eRc;
}

U24 u24MenuGetTime(void)
{
  return (_u8Hour * (U24) SECS_IN_HOUR + _u8Minute * SECS_IN_MIN);
}
