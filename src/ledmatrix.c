#include "common.h"
#include "init.h"
#include "display.h"
#include "dcf.h"
#include "menu.h"
#include "date.h"

//typedefs
typedef enum
{
  eDISP_TIME,
  eDISP_MENU,
  eDISP_SECOND,
  eDISP_DAY,
  eDISP_YEAR
}TE_DISPLAY_STATE;

//variables
static volatile U8  _u8ClkInt = 0;
static volatile bit _bDCF     = 0,
					_bSWMenu  = 0,
					_bSWSet   = 0;

static void interrupt _vInterrupt(void)
{
  if (T0IF)
  {
    //we have an interrupt on T0
    T0IF = 0;

    _u8ClkInt++;
  }

  if (RBIF)
  {
    //we need to read from RB0 before disabling the
    //interrupt since it would otherwise be raised continually
    PORTB;
    RBIF = 0;

    _bDCF    = DCF_IN;
    _bSWMenu = SW_MENU;
    _bSWSet  = SW_SET;
  }
}

void main(void)
{
  static bit _bLastSWMenu  = 0,
             _bLastSWSet   = 0;

  vInit();

  _bLastSWMenu = 1;
  _bLastSWSet  = 1;

  while (1)
  {
    static bit              _bValidTime    = 0,                      //is the time taht is currently running valid
                            _bTimeWasValid = 0,                      //time is invalidated at 4 o'clock - did we have a valid time
                            _bLastDCF      = 0;                      //last value which was input on DCF pin
    static TE_DISPLAY_STATE _eDisplayState = eDISP_TIME;             //what is to be displayed on the display
    static U24              _u24SecsToday  = 0;                      //hold the second of the day
    static TS_DATE          _stDate        = {1, 1, 10};             //hold the current date
    static U8               _u8DCFSecs     = 0;                      //how many second wraps since last DCF in state change
    static TE_MENU_CONFIG   _eConfig       = eMENU_CONF_DREIVIERTEL; //hold the config of how the time is to be displayed
    U8                      u8CurClockInt  = _u8ClkInt,              //what is the current clock interrupt count
                            u8DisplayUpd   = 0;                      //do we need to update the display content
    TE_MENU_RC              eMenuRc        = eMENU_RC_OK;            //what does the menu system tell us

    //clear the watchdog timer
    CLRWDT();

    if (u8CurClockInt >= WRAPS_A_SEC)
    {
      //we need to go back a second with the clock interrupt counter
      _u8ClkInt -= WRAPS_A_SEC;

      //increment DCF seconds timer for calculation of delay between edges
      if (!(_u8DCFSecs & 4))
      {
        //we do not need to increment any further since the delay is already too high
        //and we do not want to wrap since this might cause issues later
        _u8DCFSecs++;
      }

      //increment time of day
      _u24SecsToday++;

      if (_u24SecsToday >= SECS_A_DAY)
      {
        //a new day has come
        _u24SecsToday = 0;

        _stDate.u8Day++;

        if (!u8IsValidDate(&_stDate))
        {
          //this is a new month
          _stDate.u8Day = 1;
          _stDate.u8Month++;

          if (_stDate.u8Month > 12)
          {
            //this is a new year
            _stDate.u8Month = 1;
            _stDate.u8Year++;
          }
        }
      }
    }

    //get the time every night at 4 o'clock
    if (_bValidTime &&
        (_u24SecsToday == (SECS_IN_HOUR * 4)))
    {
      //we need to invalidate the time here
      //since showing the current time does not allow correct DCF receiving
      //interruptions would disturb the DCF input signals
      _bTimeWasValid = 1;
      _bValidTime    = 0;
    }

    //automatically set the clock if we do not have a valid time
    if (!_bValidTime)
    {
      DCF_POWER   = 1;
    }

    //has there been a change on the DCF input
    if (_bLastDCF != _bDCF)
    {
      static U8 _u8LastClkInt = 0;
      U24       u24TimeSince;

      TE_DCF_RC eDCFRc = eDCF_OK;

      _bLastDCF = _bDCF;

      if (_u8DCFSecs)
      {
        //time is the rest of the last clock interrupt to the following wrap plus
        //(wraps - 1) * 1000ms plus
        //what has elapsed of the current second
        u24TimeSince  = ((WRAPS_A_SEC - _u8LastClkInt) * (U24) 1000) / WRAPS_A_SEC;
        u24TimeSince += (_u8DCFSecs - 1) * (U24) 1000;
        u24TimeSince += (u8CurClockInt * (U24) 1000) / WRAPS_A_SEC;
      }
      else
      {
        u24TimeSince = (((u8CurClockInt - _u8LastClkInt) * 250) / WRAPS_A_SEC) * 4;
      }

      _u8DCFSecs    = 0;
      _u8LastClkInt = u8CurClockInt;

      //add a bit to the DCF calculation and 
      eDCFRc = eDCFAddBit(_bLastDCF, (U16) u24TimeSince, &_u24SecsToday, &_stDate);

      if (eDCFRc == eDCF_TIME_SET)
      {
        _bValidTime = 1;

        //disable the DCF receiver
        DCF_POWER   = 0;
      }
      else if (_bTimeWasValid &&
               _u24SecsToday > SECS_IN_HOUR * 5)
      {
        //we have tried to update the time for an hour now -> give up
        _bValidTime = 1;
        DCF_POWER   = 0;
      }
    }

    //has someone pushed the menu button
    if (_bLastSWMenu != _bSWMenu)
    {
      _bLastSWMenu = _bSWMenu;

      //only handle button release
      if (_bLastSWMenu)
      {
        eMenuRc = eHandleButton(eBUTTON_MENU, _u24SecsToday, &_stDate, _eConfig);
      }
    }
    
    //has someone pushed the set button
    else if (_bLastSWSet != _bSWSet)
    {
      _bLastSWSet = _bSWSet;
      
      //only handle button release
      if (_bLastSWSet)
      {
        eMenuRc = eHandleButton(eBUTTON_SET, _u24SecsToday, &_stDate, _eConfig);
      }
    }

    if (eMenuRc == eMENU_RC_CONFIG_CHANGED)
    {
      _eConfig = eMenuGetConfig();
    }
    else if (eMenuRc == eMENU_RC_TIME_AVAIL)
    {
      _u24SecsToday = u24MenuGetTime();
      _bValidTime   = 1;
    }
    else if (eMenuRc == eMENU_RC_DATE_AVAIL)
    {
      vMenuGetDate(&_stDate);
    }
    else if (eMenuRc == eMENU_RC_TIME_INVALID)
    {
      _bValidTime     = 0;
      _u24SecsToday   = 0;
      _stDate.u8Day   = 1;
      _stDate.u8Month = 1;
      _stDate.u8Year  = 10;
    }
    else if (eMenuRc == eMENU_RC_SHOW_SECOND)
    {
      _eDisplayState = eDISP_SECOND;
      vClearPattern();
      u8DisplayUpd   = 1;
    }
    else if (eMenuRc == eMENU_RC_SHOW_DAY)
    {
      _eDisplayState = eDISP_DAY;
      vClearPattern();
      u8DisplayUpd   = 1;
    }
    else if (eMenuRc == eMENU_RC_SHOW_YEAR)
    {
      _eDisplayState = eDISP_YEAR;
      vClearPattern();
      u8DisplayUpd   = 1;
    }
    else if (eMenuRc == eMENU_RC_ENTERED)
    {
      _eDisplayState = eDISP_MENU;
    }
    else if (eMenuRc == eMENU_RC_LEFT)
    {
      _eDisplayState = eDISP_TIME;
    }

    ROW10 = ((_eDisplayState == eDISP_TIME) && !_bValidTime) ? 1 : 0;

    if (_eDisplayState == eDISP_MENU)
    {
      //write the preconfigured menu
      vWritePattern();
    }
    else if (_eDisplayState == eDISP_SECOND)
    {
      //show current second
      static U24 _u24LastSec;

      if (u8DisplayUpd ||
          _u24LastSec != _u24SecsToday)
      {
        vAddNumToPattern((_u24SecsToday % 60) / 10, 2, 3);
        vAddNumToPattern((_u24SecsToday % 60) % 10, 6, 3);

        _u24LastSec = _u24SecsToday;
      }

      vWritePattern();
    }
    else if (_eDisplayState == eDISP_DAY)
    {
      //show day and month
      static TS_DATE _stLastDate;

      if (u8DisplayUpd ||
          _stLastDate.u8Day   != _stDate.u8Day ||
          _stLastDate.u8Month != _stDate.u8Month)
      {
        vAddNumToPattern(_stDate.u8Day   / 10, 2, 0);
        vAddNumToPattern(_stDate.u8Day   % 10, 6, 0);
        vAddNumToPattern(_stDate.u8Month / 10, 2, 6);
        vAddNumToPattern(_stDate.u8Month % 10, 6, 6);

        _stLastDate = _stDate;
      }

      vWritePattern();
    }
    else if (_eDisplayState == eDISP_YEAR)
    {
      //show current year 20xx
      static TS_DATE _stLastDate;

      if (u8DisplayUpd ||
          _stLastDate.u8Year != _stDate.u8Year)
      {
        vAddNumToPattern(2                  , 2, 0);
        vAddNumToPattern(0                  , 6, 0);
        vAddNumToPattern(_stDate.u8Year / 10, 2, 6);
        vAddNumToPattern(_stDate.u8Year % 10, 6, 6);

        _stLastDate = _stDate;
      }

      vWritePattern();
    }
    else if (_bValidTime &&
             _eDisplayState == eDISP_TIME)
    {
      vWriteTime(_u24SecsToday, _eConfig);
    }
    else
    {
      COL10 = _bLastDCF;
    }
  }
}
