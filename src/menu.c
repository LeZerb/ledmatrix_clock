#include "common.h"
#include "display.h"
#include "menu.h"

//typedefs
typedef enum
{
  //do not change menu item order
  eMENU_ITEM_NIRVANA,
  eMENU_ITEM_TIME,
  eMENU_ITEM_HOUR,
  eMENU_ITEM_MINUTE,
  eMENU_ITEM_DATE,
  eMENU_ITEM_DAY,
  eMENU_ITEM_MONTH,
  eMENU_ITEM_YEAR,
  eMENU_ITEM_INVALIDATE_TIME,
  eMENU_ITEM_TEST_DISPLAY,
  eMENU_ITEM_SHOW_SECOND,
  eMENU_ITEM_SHOW_DAY,
  eMENU_ITEM_SHOW_YEAR,
  eMENU_ITEM_COUNT
}TE_MENU_STATE;

typedef enum
{
  eMENU_ACTION_NO_ACTION,
  eMENU_ACTION_CHANGE_STATE,
  eMENU_ACTION_DO_STUFF
}TE_MENU_ACTION;

typedef struct
{
  TE_MENU_ACTION eAction;
  TE_MENU_STATE  eNextMenuState;
  TE_MENU_RC     eRc;
}TS_MENU_ACTIONS;

//variables

//what do we want to do in a certain state when a button is pressed
static const TS_MENU_ACTIONS _astMenuActions[eMENU_ITEM_COUNT][eBUTTON_COUNT] =
{
  //eMENU_ITEM_NIRVANA
  {{eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_TIME           , eMENU_RC_ENTERED}   ,
   {eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_SHOW_SECOND    , eMENU_RC_OK}},
  //eMENU_ITEM_TIME
  {{eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_DATE           , eMENU_RC_OK}        ,
  {eMENU_ACTION_CHANGE_STATE , eMENU_ITEM_HOUR           , eMENU_RC_OK}},
  //eMENU_ITEM_HOUR
  {{eMENU_ACTION_DO_STUFF    , eMENU_ITEM_HOUR           , eMENU_RC_OK}        ,
   {eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_MINUTE         , eMENU_RC_OK}},
  //eMENU_ITEM_MINUTE
  {{eMENU_ACTION_DO_STUFF    , eMENU_ITEM_MINUTE         , eMENU_RC_OK}        ,
   {eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_TIME           , eMENU_RC_TIME_AVAIL}},
  //eMENU_ITEM_DATE
  {{eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_INVALIDATE_TIME, eMENU_RC_OK}        ,
   {eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_DAY            , eMENU_RC_OK}},
  //eMENU_ITEM_DAY
  {{eMENU_ACTION_DO_STUFF    , eMENU_ITEM_DAY            , eMENU_RC_OK}        ,
   {eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_MONTH          , eMENU_RC_OK}},
  //eMENU_ITEM_MONTH
  {{eMENU_ACTION_DO_STUFF    , eMENU_ITEM_MONTH          , eMENU_RC_OK}        ,
   {eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_YEAR           , eMENU_RC_OK}},
  //eMENU_ITEM_YEAR
  {{eMENU_ACTION_DO_STUFF    , eMENU_ITEM_YEAR           , eMENU_RC_OK}        ,
   {eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_DATE           , eMENU_RC_DATE_AVAIL}},
  //eMENU_ITEM_INVALIDATE_TIME
  {{eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_TEST_DISPLAY   , eMENU_RC_OK}        ,
   {eMENU_ACTION_NO_ACTION   , eMENU_ITEM_DATE           , eMENU_RC_TIME_INVALID}},
  //eMENU_ITEM_TEST_DISPLAY
  {{eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_NIRVANA        , eMENU_RC_LEFT}      ,
   {eMENU_ACTION_DO_STUFF    , eMENU_ITEM_TEST_DISPLAY   , eMENU_RC_OK}},
  //eMENU_ITEM_SHOW_SECOND
  {{eMENU_ACTION_NO_ACTION   , eMENU_ITEM_SHOW_SECOND    , eMENU_RC_OK}        ,
   {eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_SHOW_DAY       , eMENU_RC_OK}},
  //eMENU_ITEM_SHOW_DAY
  {{eMENU_ACTION_NO_ACTION   , eMENU_ITEM_SHOW_DAY       , eMENU_RC_OK}        ,
   {eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_SHOW_YEAR      , eMENU_RC_OK}},
  //eMENU_ITEM_SHOW_YEAR
  {{eMENU_ACTION_NO_ACTION   , eMENU_ITEM_SHOW_YEAR      , eMENU_RC_OK}        ,
   {eMENU_ACTION_CHANGE_STATE, eMENU_ITEM_NIRVANA        , eMENU_RC_LEFT}}
};

static TE_MENU_STATE _eCurMenuState = eMENU_ITEM_NIRVANA;
static U8            _u8Hour        = 0,
                     _u8Minute      = 0;
static TS_DATE       _stDate        = {1, 1, 10};

//implementation

TE_MENU_RC eHandleButton(TE_BUTTONS eButton, U24 u24CurTime, TS_DATE *pstCurDate)
{
  TE_MENU_STATE eMenuStateBefore = _eCurMenuState;

  if (eButton >= eBUTTON_COUNT)
  {
    return eMENU_RC_ERROR;
  }

  if (_astMenuActions[_eCurMenuState][eButton].eAction == eMENU_ACTION_DO_STUFF)
  {
    switch (_eCurMenuState)
    {
      case eMENU_ITEM_DAY:
        {
          //increment day
          _stDate.u8Day++;
    
          if (_stDate.u8Day == 32)
          {
            _stDate.u8Day = 1;
          }
    
          vAddNumToPattern(_stDate.u8Day / 10, 2, 3);
          vAddNumToPattern(_stDate.u8Day % 10, 6, 3);
        }
        break;

      case eMENU_ITEM_MONTH:
        {
          //increment day
          _stDate.u8Month++;
    
          if (_stDate.u8Month == 13)
          {
            _stDate.u8Month = 1;
          }
    
          vAddNumToPattern(_stDate.u8Month / 10, 2, 3);
          vAddNumToPattern(_stDate.u8Month % 10, 6, 3);
        }
        break;

      case eMENU_ITEM_YEAR:
        {
          //increment year
          _stDate.u8Year++;
    
          if (_stDate.u8Year == 50)
          {
            _stDate.u8Year = 10;
          }
    
          vAddNumToPattern(_stDate.u8Year / 10, 2, 3);
          vAddNumToPattern(_stDate.u8Year % 10, 6, 3);
        }
        break;

      case eMENU_ITEM_HOUR:
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
        break;

      case eMENU_ITEM_MINUTE:
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
        break;

      case eMENU_ITEM_TEST_DISPLAY:
        {
          //perform the display test and stay on current item
          vTestDisplay();
        }
        break;

      default:
        break;
    }
  }

  if (_eCurMenuState != _astMenuActions[_eCurMenuState][eButton].eNextMenuState)
  {
    _eCurMenuState = _astMenuActions[_eCurMenuState][eButton].eNextMenuState;

    switch (_eCurMenuState)
    {
      case eMENU_ITEM_NIRVANA:
        {
          vClearPattern();
        }
        break;

      case eMENU_ITEM_TIME:
        {
          vClearPattern();
          vSetInPattern(0, 0, 1);
        }
        break;

      case eMENU_ITEM_HOUR:
        {
          vSetInPattern(1, 0, 1);
          vAddNumToPattern(_u8Hour / 10, 2, 3);
          vAddNumToPattern(_u8Hour % 10, 6, 3);
        }
        break;

      case eMENU_ITEM_MINUTE:
        {
          vSetInPattern(2, 0, 1);
          vAddNumToPattern(_u8Minute / 10, 2, 3);
          vAddNumToPattern(_u8Minute % 10, 6, 3);
        }
        break;

      case eMENU_ITEM_DATE:
        {
          //check if the configured date is valid
          if (!u8IsValidDate(&_stDate))
          {
            //this month does not have the day selected (29, 30, 31)
            _stDate.u8Day--;
    
            while (!u8IsValidDate(&_stDate))
            {
              _stDate.u8Day--;
            }
    
            //overwrite current menu item and go back to configuring the day
            _eCurMenuState = eMENU_ITEM_DAY;
    
            vSetInPattern(2, 1, 0);
            vSetInPattern(3, 1, 0);
            vAddNumToPattern(_stDate.u8Day / 10, 2, 3);
            vAddNumToPattern(_stDate.u8Day % 10, 6, 3);
          }
          else
          {
            vClearPattern();
            vSetInPattern(0, 1, 1);
          }
        }
        break;

      case eMENU_ITEM_DAY:
        {
          vSetInPattern(1, 1, 1);
          vAddNumToPattern(_stDate.u8Day / 10, 2, 3);
          vAddNumToPattern(_stDate.u8Day % 10, 6, 3);
        }
        break;

      case eMENU_ITEM_MONTH:
        {
          vSetInPattern(2, 1, 1);
          vAddNumToPattern(_stDate.u8Month / 10, 2, 3);
          vAddNumToPattern(_stDate.u8Month % 10, 6, 3);
        }
        break;

      case eMENU_ITEM_YEAR:
        {
          vSetInPattern(3, 1, 1);
          vAddNumToPattern(_stDate.u8Year / 10, 2, 3);
          vAddNumToPattern(_stDate.u8Year % 10, 6, 3);
        }
        break;

      case eMENU_ITEM_INVALIDATE_TIME:
        {
          vClearPattern();
          vSetInPattern(0, 9, 1);
        }
        break;

      case eMENU_ITEM_TEST_DISPLAY:
        {
          vClearPattern();
          vSetInPattern(0, 10, 1);
        }
        break;

      default:
        break;
    }
  }

  return _astMenuActions[eMenuStateBefore][eButton].eRc;
}

U24 u24MenuGetTime(void)
{
  return (_u8Hour * (U24) SECS_IN_HOUR + _u8Minute * SECS_IN_MIN);
}

void vMenuGetDate(TS_DATE *pstDate)
{
  if (!pstDate)
  {
    return;
  }

  pstDate->u8Day   = _stDate.u8Day;
  pstDate->u8Month = _stDate.u8Month;
  pstDate->u8Year  = _stDate.u8Year;
}