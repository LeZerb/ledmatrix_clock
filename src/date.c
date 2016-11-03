#include "common.h"
#include "date.h"

//months will be indexed beginning on index 1
static const U8 _au8DaysInMonth[13] = {
    // J   F   M   A   M   J   J   A   S   O   N   D
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

U8 u8IsValidDate(TS_DATE *pstDate) {
    if ((pstDate->u8Day == 29) &&
            (pstDate->u8Month == 2) &&
            (!(pstDate->u8Year % 4))) {
        //february the 29th in a leap year
        return 1;
    } else if (pstDate->u8Day > _au8DaysInMonth[pstDate->u8Month]) {
        //date is not valid
        return 0;
    }

    //date is valid
    return 1;
}
