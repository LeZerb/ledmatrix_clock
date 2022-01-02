#include <stdlib.h>
#include "common.h"
#include "init.h"
#include "display.h"
#include "dcf.h"
#include "menu.h"
#include "date.h"
#include "snake.h"

//typedefs

typedef enum {
    eDISP_TIME,
    eDISP_MENU,
    eDISP_TIME_DIG,
    eDISP_SECOND,
    eDISP_DAY,
    eDISP_YEAR
} TE_DISPLAY_STATE;

//variables
static volatile U8 _u8ClkInt = 0;
static volatile BOOL _bDCF = FALSE,
        _bSWMenu = FALSE,
        _bSWSet = FALSE;

static TS_TIME _stTime = {0, 0, 0}; //hold the time of the day
static TS_DATE _stDate = {1, 1, 16}; //hold the current date
static BOOL _bValidTime = FALSE; //is the time that is currently running valid

static void __interrupt _vInterrupt(void) {
    if (T0IF) {
        //we have an interrupt on T0
        T0IF = 0;

        ++_u8ClkInt;
    }

    if (RBIF) {
        //we need to read from RB0 before disabling the
        //interrupt since it would otherwise be raised continually
        PORTB;
        RBIF = 0;

        _bDCF = DCF_IN;
        _bSWMenu = SW_MENU;
        _bSWSet = SW_SET;
    }
}

void timeInvalidate() {
    _bValidTime = FALSE;
}

void timeGet(TS_TIME *time) {
    time->u8Hour = _stTime.u8Hour;
    time->u8Minute = _stTime.u8Minute;
    time->u8Second = _stTime.u8Second;
}

void timeSet(TS_TIME *time) {
    _stTime.u8Hour = time->u8Hour;
    _stTime.u8Minute = time->u8Minute;
    _stTime.u8Second = time->u8Second;
    _bValidTime = TRUE;
}

void dateGet(TS_DATE *date) {
    date->u8Day = _stDate.u8Day;
    date->u8Month = _stDate.u8Month;
    date->u8Year = _stDate.u8Year;
}

void dateSet(TS_DATE *date) {
    _stDate.u8Day = date->u8Day;
    _stDate.u8Month = date->u8Month;
    _stDate.u8Year = date->u8Year;
}

static U8 _startMs[eMS_COUNT_NUM] = {0};

U16 msSinceLastStart(TE_MS_COUNT eMsCount) {
    // There is one timer interrupt each 1000 / WRAPS_A_SEC ms
    return (U16)((((U32)(_u8ClkInt - _startMs[eMsCount])) * 1000ul) / WRAPS_A_SEC);
}

void msStart(TE_MS_COUNT eMsCount) {
    _startMs[eMsCount] = _u8ClkInt;
}

void main(void) {
    static BOOL _bLastSWMenu = FALSE, _bLastSWSet = FALSE;
    static BOOL _pendingMenu = FALSE, _pendingSet = FALSE;

    vInit();

    _bLastSWMenu = TRUE;
    _pendingMenu = TRUE;

    vSetBrightness((configGet() & eCONF_BRIGHTNESS) >> eCONF_BRIGHTNESS_SHIFT);

    while (1) {
        static BOOL _bNightlyUpdate = FALSE, //time is invalidated at 4 o'clock - did we have a valid time
                _bLastDCF = FALSE; //last value which was input on DCF pin
        static TE_DISPLAY_STATE _eDisplayState = eDISP_TIME; //what is to be displayed on the display
        static U8 _u8DCFSecs = 0; //how many second wraps since last DCF in state change
        U8 u8CurClockInt = _u8ClkInt, //what is the current clock interrupt count
                u8DisplayUpd = 0; //do we need to update the display content
        static TE_MENU_STATE _lastMenuState = eMENU_ENTRY_COUNT;
        TE_MENU_STATE eMenuState;

        //clear the watchdog timer
        CLRWDT();

        if (u8CurClockInt >= WRAPS_A_SEC) {
            //we need to go back a second with the clock interrupt counter
            _u8ClkInt -= WRAPS_A_SEC;

            //increment DCF seconds timer for calculation of delay between edges
            if (_u8DCFSecs < 4) {
                //we do not need to increment any further since the delay is already too high
                //and we do not want to wrap since this might cause issues later
                ++_u8DCFSecs;
            }

            //increment time of day
            ++(_stTime.u8Second);

            if (_stTime.u8Second >= 60) {
                //this is a new minute
                _stTime.u8Second = 0;
                ++(_stTime.u8Minute);

                if (_stTime.u8Minute >= 60) {
                    //this is a new hour
                    _stTime.u8Minute = 0;
                    ++(_stTime.u8Hour);

                    //get the time every night at 4 o'clock even though we have a valid time
                    if (_bValidTime &&
                            _stTime.u8Hour == 4 &&
                            _stTime.u8Minute == 0) {
                        //we need to invalidate the time here
                        //since showing the current time does not allow correct DCF reception
                        //interruptions would disturb the DCF input signals
                        _bNightlyUpdate = TRUE;
                        _bValidTime = FALSE;
                    }

                    if (_stTime.u8Hour >= 24) {
                        //a new day has come
                        _stTime.u8Hour = 0;
                        ++(_stDate.u8Day);

                        if (!u8IsValidDate(&_stDate)) {
                            //this is most likely a new month
                            _stDate.u8Day = 1;
                            ++(_stDate.u8Month);

                            if (_stDate.u8Month > 12) {
                                //this is a new year
                                _stDate.u8Month = 1;
                                ++(_stDate.u8Year);
                            }
                        }
                    }
                }
            }
        }

        //automatically set the clock if we do not have a valid time
        if (!_bValidTime) {
            DCF_POWER = 1;

            //has there been a change on the DCF input
            if (_bLastDCF != _bDCF) {
                static U8 _u8LastClkInt = 0;
                U32 u32TimeSince;

                TE_DCF_RC eDCFRc = eDCF_OK;

                _bLastDCF = _bDCF;

                if (_u8DCFSecs) {
                    //time is the rest of the last clock interrupt to the following wrap plus
                    //(wraps - 1) * 1000ms plus
                    //what has elapsed of the current second
                    u32TimeSince = ((WRAPS_A_SEC - _u8LastClkInt) * (U32) 1000) / WRAPS_A_SEC;
                    u32TimeSince += (_u8DCFSecs - 1) * (U32) 1000;
                    u32TimeSince += (u8CurClockInt * (U32) 1000) / WRAPS_A_SEC;
                } else {
                    u32TimeSince = (((u8CurClockInt - _u8LastClkInt) * (U32) 250) / WRAPS_A_SEC) * 4;
                }

                _u8DCFSecs = 0;
                _u8LastClkInt = u8CurClockInt;

                //add a bit to the DCF calculation and u32TimeSince should fit in 16bit after the calculation is done
                eDCFRc = eDCFAddBit(_bLastDCF, (U16) u32TimeSince, &_stTime, &_stDate);

                if (eDCFRc == eDCF_TIME_SET) {
                    _bValidTime = TRUE;

                    //disable the DCF receiver
                    DCF_POWER = 0;
                } else if (_bNightlyUpdate &&
                        _stTime.u8Hour >= 5) {
                    //we have tried to update the time for an hour now -> give up
                    _bNightlyUpdate = FALSE;
                    _bValidTime = TRUE;
                    DCF_POWER = 0;
                }
            }
        }

        //has someone pushed the menu button
        if (_bLastSWMenu != _bSWMenu) {
            _bLastSWMenu = _bSWMenu;

            _pendingMenu = _bLastSWMenu;

            // Only handle button release
            if (_bLastSWMenu) {
                msStart(eMS_COUNT_BUTTON_MENU);
            }
        }

        //has someone pushed the set button
        if (_bLastSWSet != _bSWSet) {
            _bLastSWSet = _bSWSet;

            _pendingSet = _bLastSWSet;

            // Only handle button release
            if (_bLastSWSet) {
                msStart(eMS_COUNT_BUTTON_SET);
            }
        }

        // Deglitch buttons
        if (_pendingMenu) {
            if (msSinceLastStart(eMS_COUNT_BUTTON_MENU) > 50) {
                eHandleButton(eBUTTON_MENU);
                _pendingMenu = FALSE;
                _pendingSet = FALSE;
            }
        } else if (_pendingSet) {
            if (msSinceLastStart(eMS_COUNT_BUTTON_SET) > 50) {
                eHandleButton(eBUTTON_SET);
                _pendingMenu = FALSE;
                _pendingSet = FALSE;
            }
        }

        eMenuState = menuGetState();

        if (_lastMenuState != eMenuState) {
            _lastMenuState = eMenuState;
            u8DisplayUpd = 1;
            if (eMenuState == eMENU_SHOW_TIME) {
                _eDisplayState = eDISP_TIME_DIG;
                vClearPattern();
            } else if (eMenuState == eMENU_SHOW_SECOND) {
                _eDisplayState = eDISP_SECOND;
                vClearPattern();
            } else if (eMenuState == eMENU_SHOW_DAY) {
                _eDisplayState = eDISP_DAY;
                vClearPattern();
            } else if (eMenuState == eMENU_SHOW_YEAR) {
                _eDisplayState = eDISP_YEAR;
                vClearPattern();
            } else if (eMenuState != eMENU_NIRVANA) {
                _eDisplayState = eDISP_MENU;
            } else {
                _eDisplayState = eDISP_TIME;
            }
        }

        //enable last row for DCF reception blinking when no valid time is set
        ROW10 = ((!_bValidTime) && (_eDisplayState == eDISP_TIME)) ? 1 : 0;

        if (eMenuState == eMENU_SNAKE) {
            snakeRun(eBUTTON_COUNT);
        }

        if (_eDisplayState == eDISP_MENU) {
            //write the preconfigured menu pattern
            vWritePattern();
        } else if (_eDisplayState == eDISP_TIME_DIG) {
            //show current time in digital
            static U8 _u8LastHour,
                    _u8LastMinute;

            if (u8DisplayUpd ||
                    _u8LastHour != _stTime.u8Hour ||
                    _u8LastMinute != _stTime.u8Minute) {
                _u8LastHour = _stTime.u8Hour;
                _u8LastMinute = _stTime.u8Minute;

                vAddNumToPattern(_u8LastHour / 10, 2, 0);
                vAddNumToPattern(_u8LastHour % 10, 6, 0);
                vAddNumToPattern(_u8LastMinute / 10, 2, 6);
                vAddNumToPattern(_u8LastMinute % 10, 6, 6);
            }

            vSetInPattern(10, 4, _stTime.u8Second & 1);
            vSetInPattern(10, 6, _stTime.u8Second & 1);

            vWritePattern();
        } else if (_eDisplayState == eDISP_SECOND) {
            //show current second
            static U8 _u8LastSec = 0xFF;

            if (u8DisplayUpd || (_u8LastSec % 10) != (_stTime.u8Second % 10)) {
                vAddNumToPattern(_stTime.u8Second % 10, 6, 3);
            }

            if (u8DisplayUpd || (_u8LastSec / 10) != (_stTime.u8Second / 10)) {
                vAddNumToPattern(_stTime.u8Second / 10, 2, 3);
            }

            _u8LastSec = _stTime.u8Second;

            vWritePattern();
        } else if (_eDisplayState == eDISP_DAY) {
            //show day and month
            static U8 _u8LastDay,
                    _u8LastMonth;

            if (u8DisplayUpd ||
                    _u8LastDay != _stDate.u8Day ||
                    _u8LastMonth != _stDate.u8Month) {
                _u8LastDay = _stDate.u8Day;
                _u8LastMonth = _stDate.u8Month;

                vAddNumToPattern(_u8LastDay / 10, 2, 0);
                vAddNumToPattern(_u8LastDay % 10, 6, 0);
                vAddNumToPattern(_u8LastMonth / 10, 2, 6);
                vAddNumToPattern(_u8LastMonth % 10, 6, 6);
            }

            vWritePattern();
        } else if (_eDisplayState == eDISP_YEAR) {
            //show current year 20xx
            static U8 _u8LastYear;

            if (u8DisplayUpd || _u8LastYear != _stDate.u8Year) {
                _u8LastYear = _stDate.u8Year;

                vAddNumToPattern(2, 2, 0);
                vAddNumToPattern(0, 6, 0);
                vAddNumToPattern(_u8LastYear / 10, 2, 6);
                vAddNumToPattern(_u8LastYear % 10, 6, 6);
            }

            vWritePattern();
        } else if (_bValidTime && _eDisplayState == eDISP_TIME) {
            vWriteTime(&_stTime, 0xFF);
        } else {
            COL10 = _bLastDCF;
        }
    }
}
