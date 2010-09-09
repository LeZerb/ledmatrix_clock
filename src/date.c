/*
	Version 1.12  Gary Katch 1999/1/6.
	fix argc=0 default behaviour.
	*/
#include <stdio.h>
#include "common.h"
#include "date.h"

//convert a date to a number
U32 u32Date2Num(TS_DATE *pstDate)
{
#if 0
  U8  u8M;
  U16 u16Y;
  long  y, m;

  if (!pstDate)
  {
    return 0;
  }

  u8M  = (pstDate->u8Month + 9) % 12;        /* mar=0, feb=11 */
  u16Y = (2000 + pstDate->u8Year) - u8M / 10; /* if Jan/Feb, year-- */
  
  return (U32) u16Y * 365 + u16Y / 4 - u16Y / 100 + u16Y / 400 + (u8M * 306 + 5) / 10 + (pstDate->u8Day - 1);
#else
  return 1;
#endif
}

//convert a number to a date
void stNum2Date(U32 u32Num, TS_DATE *pstDate)
{
#if 0
  TS_DATE stDate;

  U32 u32Y, u32D, u32Mi;

  u32Y = (10000 * u32Num + 14780) / 3652425;

  u32D = u32Num - (u32Y * 365 + u32Y / 4 - u32Y / 100 + u32Y / 400);

  if (u32D < 0)
  {
    u32Y--;
    u32D = u32Num - (u32Y * 365 + u32Y / 4 - u32Y / 100 + u32Y / 400);
  }

  u32Mi = (52 + 100 * u32D) / 3060;

  stDate.u8Year  = (U8)((u32Y + (u32Mi + 2) / 12) - 2000);
  stDate.u8Month = (U8)((u32Mi + 2) % 12 + 1);
  stDate.u8Day   = (U8)(u32D - (u32Mi * 306 + 5) / 10 + 1);

  pstDate-> = ;
#endif
}

U8 u8IsValidDate(TS_DATE *pstDate)
{
#if 0
  TS_DATE stDate;
  U32     u32Num;

  u32Num = u32Date2Num(&stDate);

  stDate = stNum2Date(u32Num);

  if ((pstDate->u8Day   == stDate.u8Day)   &&
      (pstDate->u8Month == stDate.u8Month) &&
      (pstDate->u8Year  == stDate.u8Year))
  {
    return 1;
  }
  else
  {
    return 0;
  }
#else
  return 1;
#endif
}
