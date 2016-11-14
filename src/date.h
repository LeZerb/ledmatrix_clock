#ifndef _DATE_H_
#define _DATE_H_

//typedefs

typedef struct {
    U8 u8Day;
    U8 u8Month;
    U8 u8Year;
} TS_DATE;

typedef struct {
    U8 u8Hour;
    U8 u8Minute;
    U8 u8Second;
} TS_TIME;

typedef enum {
    eMS_COUNT_BUTTON_MENU,
    eMS_COUNT_BUTTON_SET,
    eMS_COUNT_SNAKE,
    eMS_COUNT_NUM
} TE_MS_COUNT;

U8 u8IsValidDate(TS_DATE *pstDate);

void timeInvalidate();
void timeGet(TS_TIME *time);
void timeSet(TS_TIME *time);
void dateGet(TS_DATE *date);
void dateSet(TS_DATE *date);
U16 msSinceLastStart(TE_MS_COUNT eMsCount);
void msStart(TE_MS_COUNT eMsCount);

#endif
