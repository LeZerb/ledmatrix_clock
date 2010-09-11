#include "common.h"
#include "dcf.h"
#include "display.h"
#include "date.h"

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

static void _vCalcDate(U8 *pu8DCFData, TS_DATE *pstDate)
{
  if (!pu8DCFData ||
      !pstDate)
  {
    return;
  }

  //reset the date since we are calculating a new date
  pstDate->u8Day   = 0;
  pstDate->u8Month = 0;
  pstDate->u8Year  = 0;

  //00000000 00000000 00000000 00000000 00001111 11000111 11111111 11000000
  //0        8        16       24       32       40       48       56
  //0        1        2        3        4        5        6        7

  //bits 36–41 code the day
  pstDate->u8Day += (pu8DCFData[4] & 0x08) ?  1 : 0;
  pstDate->u8Day += (pu8DCFData[4] & 0x04) ?  2 : 0;
  pstDate->u8Day += (pu8DCFData[4] & 0x02) ?  4 : 0;
  pstDate->u8Day += (pu8DCFData[4] & 0x01) ?  8 : 0;
  pstDate->u8Day += (pu8DCFData[5] & 0x80) ? 10 : 0;
  pstDate->u8Day += (pu8DCFData[5] & 0x40) ? 20 : 0;

  //bits 45–49 code the month
  pstDate->u8Month += (pu8DCFData[5] & 0x04) ?  1 : 0;
  pstDate->u8Month += (pu8DCFData[5] & 0x02) ?  2 : 0;
  pstDate->u8Month += (pu8DCFData[5] & 0x01) ?  4 : 0;
  pstDate->u8Month += (pu8DCFData[6] & 0x80) ?  8 : 0;
  pstDate->u8Month += (pu8DCFData[6] & 0x40) ? 10 : 0;

  //bits 50–57 code the year
  pstDate->u8Year += (pu8DCFData[6] & 0x20) ?  1 : 0;
  pstDate->u8Year += (pu8DCFData[6] & 0x10) ?  2 : 0;
  pstDate->u8Year += (pu8DCFData[6] & 0x08) ?  4 : 0;
  pstDate->u8Year += (pu8DCFData[6] & 0x04) ?  8 : 0;
  pstDate->u8Year += (pu8DCFData[6] & 0x02) ? 10 : 0;
  pstDate->u8Year += (pu8DCFData[6] & 0x01) ? 20 : 0;
  pstDate->u8Year += (pu8DCFData[7] & 0x80) ? 40 : 0;
  pstDate->u8Year += (pu8DCFData[7] & 0x40) ? 80 : 0;
}

TE_DCF_RC eDCFAddBit(U8 u8Edge, U16 u16After, U24 *pu24Time, TS_DATE *pstDate)
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
      static U24     _u24FirstTime = 0;
      static TS_DATE _stFirstDate  = {1, 1, 10};

      //a sync event has occurred
      _bSyncDone = 1;

      if (_u8DCFBitsRecvd == 59)
      {
        //we can now set the time now
        _vCalcTime(_au8DCFData, &_u24FirstTime);
        _vCalcDate(_au8DCFData, &_stFirstDate);
      }
      else if (_u8DCFBitsRecvd == (2 * 59))
      {
        U24     u24TimeNow;
        TS_DATE stDateNow;

        _vCalcTime(&_au8DCFData[(59 / 8) + 1], &u24TimeNow);
        _vCalcDate(&_au8DCFData[(59 / 8) + 1], &stDateNow);
        
        //we need to check if the time matches the previous time
        //and if it does we can set the time
        if (((_u24FirstTime + SECS_IN_MIN) == u24TimeNow) &&
            (_stFirstDate.u8Day   == stDateNow.u8Day)   &&
            (_stFirstDate.u8Month == stDateNow.u8Month) &&
            (_stFirstDate.u8Year  == stDateNow.u8Year))
        {
          *pu24Time = u24TimeNow;
          *pstDate  = stDateNow;
          eRc = eDCF_TIME_SET;
        }
        else
        {
          //copy the last DCF data set to the start and wait for a second set of data
          memcpy(_au8DCFData, &_au8DCFData[(59 / 8) + 1], (59 / 8) + 1);
          //set these 59 bits as valid
          _u8DCFBitsRecvd = 59;
          //remember the time that was calculated just now for the next comparison
          _u24FirstTime   = u24TimeNow;
          _stFirstDate    = stDateNow;
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

      BIT_SET_8(_au8DCFData[_u8CurrentByte], _u8CurrentBit);

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
    }
  }

  return eRc;
}
