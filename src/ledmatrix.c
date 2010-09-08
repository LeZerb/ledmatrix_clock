#include "common.h"
#include "init.h"
#include "display.h"
#include "dcf.h"
#include "menu.h"

//defines
#ifndef DEBUG
  #define DEBUG 0
#elif (DEBUG == DEBUG_INPUTS)
  #undef  DEBUG
  #define DEBUG 1
#else
  #undef  DEBUG
  #define DEBUG 0
#endif

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
    static bit _bValidTime   = 0,
               _bLastDCF     = 0,
               _bInMenu      = 0,
               _bShowSecond  = 0;
    static U24 _u24SecsToday = 0;
    static U8  _u8DCFSecs    = 0;
    U8         u8CurClockInt = _u8ClkInt;
    TE_MENU_RC eMenuRc = eMENU_OK;

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
      }
    }

    if (//automatically set the clock if we do not have the current time
        !_bValidTime ||
        //and every night at 4 o'clock
        (_u24SecsToday == (SECS_IN_HOUR * 4)))
    {
      DCF_POWER = 1;
    }

    if (_bLastDCF != _bDCF)
    {
      //there was a change on the DCF input
      static U8 _u8LastClkInt = 0;
      U24       u24TimeSince;

      TE_DCF_RC eDCFRc = eDCF_OK;

      #if DEBUG
        COL10 = !COL10;
      #endif

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

      eDCFRc = eDCFAddBit(_bValidTime ? eDCF_UPDATE_TIME : eDCF_GET_INITIAL_TIME,
                          _bLastDCF, (U16) u24TimeSince, &_u24SecsToday);

      if (eDCFRc == eDCF_TIME_SET)
      {
        _bValidTime = 1;
      }
      else if (eDCFRc == eDCF_ALL_DONE)
      {
        _bValidTime = 1;

        //disable the DCF receiver
        DCF_POWER   = 0;
      }
      else if (_bValidTime &&
               _u24SecsToday > SECS_IN_HOUR * 5)
      {
        //we have tried to update the time for an hour now -> give up
        DCF_POWER   = 0;
      }
    }

    if (_bLastSWMenu != _bSWMenu)
    {
      _bLastSWMenu = _bSWMenu;

      if (_bLastSWMenu)
      {
        #if DEBUG
          COL8 = !COL8;
        #endif

        eMenuRc = eHandleButton(eBUTTON_MENU);
      }
    }

    if (_bLastSWSet != _bSWSet)
    {
      _bLastSWSet = _bSWSet;

      if (_bLastSWSet)
      {
        #if DEBUG
          COL9 = !COL9;
        #endif

        eMenuRc = eHandleButton(eBUTTON_SET);
      }
    }

    if (eMenuRc == eMENU_TIME_AVAIL)
    {
      _u24SecsToday = u24MenuGetTime();
      _bValidTime   = 1;
    }
    else if (eMenuRc == eMENU_TIME_INVALID)
    {
      _bValidTime = 0;
    }
    else if (eMenuRc == eMENU_TOGGLE_SECOND)
    {
      _bShowSecond = !_bShowSecond;
      vClearPattern();
    }
    else if (eMenuRc == eMENU_ENTERED)
    {
      _bInMenu = 1;
    }
    else if (eMenuRc == eMENU_LEFT)
    {
      _bInMenu = 0;
    }

    #if DEBUG
      ROW0 = 1;
    #else
      if (_bInMenu)
      {
        vWritePattern();
      }
      else if (_bShowSecond)
      {
        //show current second
        vAddNumToPattern((_u24SecsToday % 60) / 10, 2, 3);
        vAddNumToPattern((_u24SecsToday % 10) % 10, 6, 3);
        vWritePattern();
      }
      else if (_bValidTime)
      {
        vWriteTime(_u24SecsToday, 1);
      }
      else
      {
        //show received DCF data on last LED
        //vSetInPattern(NUM_COLS - 1, NUM_ROWS - 1, _bLastDCF);
        //vWritePattern();

        ROW11 = 1;
        COL10 = _bLastDCF;
      }
    #endif
  }
}
