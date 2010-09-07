#include "common.h"
#include "dcf.h"
#include "display.h"

#ifndef DEBUG
  #define DEBUG 0
#elif (DEBUG == DEBUG_DCF)
  #undef  DEBUG
  #define DEBUG 1
#else
  #undef  DEBUG
  #define DEBUG 0
#endif

void _vCalcTime(U8 *pu8DCFData, U24 *pu24Time)
{
  U8 u8Minute = 0, u8Hour = 0;

  if (!pu8DCFData ||
      !pu24Time)
  {
    return;
  }

  //00000000 00000000 00000111 11110111 11100000
  //0        8        16       24       32
  //0        1        2        3        4

  //bits 21-27 code the minutes
  u8Minute += (pu8DCFData[2] & 0x04) ?  1 : 0;
  u8Minute += (pu8DCFData[2] & 0x02) ?  2 : 0;
  u8Minute += (pu8DCFData[2] & 0x01) ?  4 : 0;
  u8Minute += (pu8DCFData[3] & 0x80) ?  8 : 0;
  u8Minute += (pu8DCFData[3] & 0x40) ? 10 : 0;
  u8Minute += (pu8DCFData[3] & 0x20) ? 20 : 0;
  u8Minute += (pu8DCFData[3] & 0x10) ? 40 : 0;

  //bits 29-34 code the hours
  u8Hour += (pu8DCFData[3] & 0x04) ?  1 : 0;
  u8Hour += (pu8DCFData[3] & 0x02) ?  2 : 0;
  u8Hour += (pu8DCFData[3] & 0x01) ?  4 : 0;
  u8Hour += (pu8DCFData[4] & 0x80) ?  8 : 0;
  u8Hour += (pu8DCFData[4] & 0x40) ? 10 : 0;
  u8Hour += (pu8DCFData[4] & 0x20) ? 20 : 0;

  *pu24Time = (U24) u8Hour * SECS_IN_HOUR + u8Minute * SECS_IN_MIN;
}

TE_DCF_RC eDCFAddBit(TE_DCF_ACTION eAction, U8 u8Edge, U16 u16After, U24 *pu24Time)
{
  TE_DCF_RC eRc = eDCF_OK;

  //we need room for two sets of DCF data
  static U8  _au8DCFData[2 * ((59 / 8) + 1)] = {0};
  static U8  _u8DCFBitsRecvd                 = 0;
  static bit _bSyncDone                      = 0;

  if (!pu24Time)
  {
    return eDCF_ERROR;
  }

#if (DCF_INVERTED)
  if (u8Edge)
#else
  if (!u8Edge)
#endif
  {
    //look for a sync event
    if (u16After > 1750 &&
        u16After < 1950)
    {
      static U24 _u24FirstTime = 0;

      //a sync event has occurred
      _bSyncDone = 1;

      #if DEBUG
        COL0 = !COL0;
      #endif

      if (_u8DCFBitsRecvd == 59)
      {
        //we can now set the time now
        _vCalcTime(_au8DCFData, &_u24FirstTime);

        if (eAction == eDCF_GET_INITIAL_TIME)
        {
          #if DEBUG
            COL3 = !COL3;
          #endif

          *pu24Time = _u24FirstTime;
          eRc = eDCF_TIME_SET;
        }
      }
      else if (_u8DCFBitsRecvd == (2 * 59))
      {
        U24 u24TimeNow;

        //we need to check if the time matches the previous time
        //and if it does we can set the time and return with return code done
        _vCalcTime(&_au8DCFData[(59 / 8) + 1], &u24TimeNow);

        if (((_u24FirstTime + SECS_IN_MIN) == u24TimeNow) ||
            ((_u24FirstTime == (SECS_A_DAY - SECS_IN_MIN)) && u24TimeNow == 0))
        {
          #if DEBUG
            COL4 = !COL4;
          #endif

          *pu24Time = u24TimeNow;
          eRc = eDCF_ALL_DONE;
        }
        else
        {
          memcpy(_au8DCFData, &_au8DCFData[(59 / 8) + 1], (59 / 8) + 1);
          _u8DCFBitsRecvd = 59;
        }
      }
      else
      {
        _u8DCFBitsRecvd = 0;
      }
    }
    else if (u16After < 750 ||
             u16After > 950)
    {
      //this is not an edge of a data bit -> error
      _u8DCFBitsRecvd = 0;
      _bSyncDone      = 0;
      eRc = eDCF_ERROR;

      #if DEBUG
        COL10 = !COL10;
      #endif
    }
  }
  else
  {
    static U8 _u8CurrentByte = 0,
              _u8CurrentBit  = 7;

    if ((_u8DCFBitsRecvd == 59 && !_bSyncDone) ||
        (_u8DCFBitsRecvd == (2 * 59)))
    {
      //we already got 59 bytes but have not seen a sync
      //or we already have all data bytes but another one is upcoming
      _u8DCFBitsRecvd = 0;
      _bSyncDone      = 0;

      #if DEBUG
        COL9 = !COL9;
      #endif
    }
    else if (_u8DCFBitsRecvd == 59)
    {
      _u8CurrentByte = (59 / 8) + 1;
      _u8CurrentBit  = 7;
    }
    
    if (!_u8DCFBitsRecvd)
    {
      _u8CurrentByte              = 0;
      _u8CurrentBit               = 7;
      _au8DCFData[_u8CurrentByte] = 0;
    }

    //this may be a data bit
    if (u16After > 50 &&
        u16After < 150)
    {
      //this is most likely a 0
      //we do not need to save this value since it is already set
      _u8DCFBitsRecvd++;
      
      #if DEBUG
        COL1 = !COL1;
      #endif

      if (_u8CurrentBit)
      {
        _u8CurrentBit--;
      }
      else
      {
        _u8CurrentBit = 7;
        _u8CurrentByte++;
        _au8DCFData[_u8CurrentByte] = 0;
      }
    }
    else if (u16After > 150 &&
             u16After < 250)
    {
      //this is most likely a 1
      _u8DCFBitsRecvd++;
      
      #if DEBUG
        COL2 = !COL2;
      #endif

      BIT_SET(_au8DCFData[_u8CurrentByte], _u8CurrentBit);

      if (_u8CurrentBit)
      {
        _u8CurrentBit--;
      }
      else
      {
        _u8CurrentBit = 7;
        _u8CurrentByte++;
        _au8DCFData[_u8CurrentByte] = 0;
      }
    }
    else
    {
      _u8DCFBitsRecvd = 0;
      _bSyncDone      = 0;
      eRc = eDCF_ERROR;
      
      #if DEBUG
        COL8 = !COL8;
      #endif
    }
  }

  {
    U8 u8CurBit, u8Byte = 0, u8Bit = 7;

    memset(au8Pattern, 0x00, PATTERN_SIZE);

    for (u8CurBit = 0; u8CurBit < _u8DCFBitsRecvd; u8CurBit++)
    {
      if ((_au8DCFData[u8Byte] >> u8Bit) & 0x1)
      {
        BIT_SET(au8Pattern[u8Byte], u8Bit);
      }

      if (u8CurBit == 59)
      {
        u8Byte = 59 / 8 + 1;
        u8Bit  = 7;
      }
      else
      {
        if (u8Bit)
        {
          u8Bit--;
        }
        else
        {
          u8Bit = 7;
          u8Byte ++;
        }
      }
    }
  }

  return eRc;
}
