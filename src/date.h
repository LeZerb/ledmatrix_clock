#ifndef _DATE_H_
#define _DATE_H_

//typedefs
typedef struct
{
  U8 u8Day;
  U8 u8Month;
  U8 u8Year;
}TS_DATE;

U8 u8IsValidDate(TS_DATE *pstDate);

#endif
