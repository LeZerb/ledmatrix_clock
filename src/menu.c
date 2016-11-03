#include "common.h"
#include "config.h"
#include "display.h"
#include "menu.h"

/*
 * set button is right (->), menu button is down (new line)
 *
 * nirvana -> show time -> show second -> show day -> show year -> nirvana
 *            show time    show second    show day    show year
 * config -> config change -> config
 *           config change
 * time -> hour -> minute -> day -> month -> year -> time
 *        +hour   +minute   +day   +month   +year
 * invalidate time
 * test display
 * nirvana
 *
 */

//where do we want to go on menu and set button presses
static const TE_MENU_STATE _aeMenuStateChanges[eMENU_ENTRY_COUNT][eBUTTON_COUNT] = {
    //eMENU_NIRVANA
    {
        eMENU_MAIN_CONFIG,
        eMENU_SHOW_TIME
    },
    //eMENU_MAIN_CONFIG
    {
        eMENU_MAIN_TIME,
        eMENU_SET_CONFIG_ES_IST
    },
    //eMENU_MAIN_TIME
    {
        eMENU_MAIN_INVALIDATE_TIME,
        eMENU_SET_HOUR
    },
    //eMENU_MAIN_INVALIDATE_TIME
    {
        eMENU_MAIN_TEST_DISPLAY,
        eMENU_MAIN_INVALIDATE_TIME
    },
    //eMENU_MAIN_TEST_DISPLAY
    {
        eMENU_NIRVANA,
        eMENU_MAIN_TEST_DISPLAY
    },
    //eMENU_SET_CONFIG_ES_IST
    {
        eMENU_SET_CONFIG_ES_IST,
        eMENU_SET_CONFIG_VIERTEL_VOR_NACH
    },
    //eMENU_SET_CONFIG_VIERTEL_VOR_NACH
    {
        eMENU_SET_CONFIG_VIERTEL_VOR_NACH,
        eMENU_SET_CONFIG_ZWANZIG_VOR_NACH
    },
    //eMENU_SET_CONFIG_ZWANZIG_VOR_NACH
    {
        eMENU_SET_CONFIG_ZWANZIG_VOR_NACH,
        eMENU_SET_CONFIG_MITTERNACHT
    },
    //eMENU_SET_CONFIG_MITTERNACHT
    {
        eMENU_SET_CONFIG_MITTERNACHT,
        eMENU_SET_CONFIG_NULL
    },
    //eMENU_SET_CONFIG_NULL
    {
        eMENU_SET_CONFIG_NULL,
        eMENU_MAIN_CONFIG
    },
    //eMENU_SET_HOUR
    {
        eMENU_SET_HOUR,
        eMENU_SET_MINUTE
    },
    //eMENU_SET_MINUTE
    {
        eMENU_SET_MINUTE,
        eMENU_SET_DAY
    },
    //eMENU_SET_DAY
    {
        eMENU_SET_DAY,
        eMENU_SET_MONTH
    },
    //eMENU_SET_MONTH
    {
        eMENU_SET_MONTH,
        eMENU_SET_YEAR
    },
    //eMENU_SET_YEAR
    {
        eMENU_SET_YEAR,
        eMENU_MAIN_TIME
    },
    //eMENU_SHOW_TIME
    {
        eMENU_NIRVANA,
        eMENU_SHOW_SECOND
    },
    //eMENU_SHOW_SECOND
    {
        eMENU_NIRVANA,
        eMENU_SHOW_DAY
    },
    //eMENU_SHOW_DAY
    {
        eMENU_NIRVANA,
        eMENU_SHOW_YEAR
    },
    //eMENU_SHOW_YEAR
    {
        eMENU_NIRVANA,
        eMENU_NIRVANA
    },
};

static TE_MENU_STATE _eCurMenuState = eMENU_NIRVANA;
static TS_TIME _time = {0, 0, 0};
static BOOL _timeChanged = FALSE;
static TS_DATE _date = {1, 1, 16};
static BOOL _dateChanged = FALSE;
static TE_CONFIG _config = 0;
static BOOL _configChanged = FALSE;
static BOOL _timeInvalidate = FALSE;

//implementation

void leaveState(TE_MENU_STATE state) {
    switch (state) {
        case eMENU_NIRVANA:
            _config = configGet();
            dateGet(&_date);
            timeGet(&_time);
            _configChanged = FALSE;
            _date.u8Day = 1;
            _date.u8Month = 1;
            _date.u8Year = 16;
            _dateChanged = FALSE;
            _time.u8Hour = 0;
            _time.u8Minute = 0;
            _time.u8Second = 0;
            _timeChanged = FALSE;
            break;
        case eMENU_MAIN_CONFIG:
            if (_configChanged) {
                configSave(_config);
            }
            break;
        case eMENU_MAIN_TIME:
            if (_timeChanged) {
                timeSet(&_time);
            }
            if (_dateChanged) {
                dateSet(&_date);
            }
            break;
        case eMENU_MAIN_INVALIDATE_TIME:
            if (_timeInvalidate) {
                timeInvalidate();
            }
            break;
    }
}

void enterState(TE_MENU_STATE state) {
    switch (state) {
        case eMENU_MAIN_CONFIG:
            vClearPattern();
            vSetInPattern(0, 10, 1);
            if (_config & eCONF_ES_IST) {
                vAddTextToPattern(eTEXT_ES_IST);
            }
            if (_config & eCONF_MITTERNACHT) {
                vAddTextToPattern(eTEXT_MITTERNACHT);
            }
            if (_config & eCONF_NULL) {
                vAddTextToPattern(eTEXT_NULL_H);
                vAddTextToPattern(eTEXT_NULL_UHR);
            }
            if (_config & eCONF_VIERTEL_VOR_NACH) {
                vAddTextToPattern(eTEXT_VIERTEL);
                vAddTextToPattern(eTEXT_VOR);
                vAddTextToPattern(eTEXT_NACH);
            } else {
                vAddTextToPattern(eTEXT_DREIVIERTEL);
            }
            if (_config & eCONF_ZWANZIG_VOR_NACH) {
                vAddTextToPattern(eTEXT_ZWANZIG);
                vAddTextToPattern(eTEXT_VOR);
                vAddTextToPattern(eTEXT_NACH);
            }
            break;
        case eMENU_MAIN_INVALIDATE_TIME:
            vClearPattern();
            vSetInPattern(0, 8, 1);
            break;
        case eMENU_MAIN_TEST_DISPLAY:
            vClearPattern();
            vSetInPattern(0, 7, 1);
            break;
        case eMENU_MAIN_TIME:
            vClearPattern();
            vSetInPattern(0, 9, 1);
            //show time 00:00
            vAddNumToPattern(_time.u8Hour / 10, 2, 0);
            vAddNumToPattern(_time.u8Hour % 10, 6, 0);
            vAddNumToPattern(_time.u8Minute / 10, 2, 6);
            vAddNumToPattern(_time.u8Minute % 10, 6, 6);
            //add colon
            vSetInPattern(10, 4, 1);
            vSetInPattern(10, 6, 1);
            break;
        case eMENU_SET_CONFIG_ES_IST:
            vClearPattern();
            vSetInPattern(0, 10, 1);
            vSetInPattern(2, 10, 1);
            if (_config & eCONF_ES_IST) {
                vAddTextToPattern(eTEXT_ES_IST);
            }
            break;
        case eMENU_SET_CONFIG_MITTERNACHT:
            vClearPattern();
            vSetInPattern(0, 10, 1);
            vSetInPattern(5, 10, 1);
            if (_config & eCONF_MITTERNACHT) {
                vAddTextToPattern(eTEXT_MITTERNACHT);
            }
            break;
        case eMENU_SET_CONFIG_NULL:
            vClearPattern();
            vSetInPattern(0, 10, 1);
            vSetInPattern(6, 10, 1);
            if (_config & eCONF_NULL) {
                vAddTextToPattern(eTEXT_NULL_H);
                vAddTextToPattern(eTEXT_NULL_UHR);
            }
            break;
        case eMENU_SET_CONFIG_VIERTEL_VOR_NACH:
            vClearPattern();
            vSetInPattern(0, 10, 1);
            vSetInPattern(3, 10, 1);
            if (_config & eCONF_VIERTEL_VOR_NACH) {
                vAddTextToPattern(eTEXT_VIERTEL);
                vAddTextToPattern(eTEXT_VOR);
                vAddTextToPattern(eTEXT_NACH);
            } else {
                vAddTextToPattern(eTEXT_DREIVIERTEL);
            }
            break;
        case eMENU_SET_CONFIG_ZWANZIG_VOR_NACH:
            vClearPattern();
            vSetInPattern(0, 10, 1);
            vSetInPattern(4, 10, 1);
            if (_config & eCONF_ZWANZIG_VOR_NACH) {
                vAddTextToPattern(eTEXT_ZWANZIG);
                vAddTextToPattern(eTEXT_VOR);
                vAddTextToPattern(eTEXT_NACH);
            }
            break;
        case eMENU_SET_DAY:
            vClearPattern();
            //show day and month
            vAddNumToPattern(_date.u8Day / 10, 2, 0);
            vAddNumToPattern(_date.u8Day % 10, 6, 0);
            vAddNumToPattern(_date.u8Month / 10, 2, 6);
            vAddNumToPattern(_date.u8Month % 10, 6, 6);
            //add dot in front of day
            vSetInPattern(0, 2, 1);
            break;
        case eMENU_SET_HOUR:
            vClearPattern();
            //show time
            vAddNumToPattern(_time.u8Hour / 10, 2, 0);
            vAddNumToPattern(_time.u8Hour % 10, 6, 0);
            vAddNumToPattern(_time.u8Minute / 10, 2, 6);
            vAddNumToPattern(_time.u8Minute % 10, 6, 6);
            //add colon
            vSetInPattern(10, 4, 1);
            vSetInPattern(10, 6, 1);
            //add dot in front of hour
            vSetInPattern(0, 2, 1);
            break;
        case eMENU_SET_MINUTE:
            vClearPattern();
            //show time
            vAddNumToPattern(_time.u8Hour / 10, 2, 0);
            vAddNumToPattern(_time.u8Hour % 10, 6, 0);
            vAddNumToPattern(_time.u8Minute / 10, 2, 6);
            vAddNumToPattern(_time.u8Minute % 10, 6, 6);
            //add colon
            vSetInPattern(10, 4, 1);
            vSetInPattern(10, 6, 1);
            //add dot in front of minute
            vSetInPattern(0, 8, 1);
            break;
        case eMENU_SET_MONTH:
            vClearPattern();
            //show day and month
            vAddNumToPattern(_date.u8Day / 10, 2, 0);
            vAddNumToPattern(_date.u8Day % 10, 6, 0);
            vAddNumToPattern(_date.u8Month / 10, 2, 6);
            vAddNumToPattern(_date.u8Month % 10, 6, 6);
            //add dot in front of month
            vSetInPattern(0, 8, 1);
            break;
        case eMENU_SET_YEAR:
            vClearPattern();
            //show year
            vAddNumToPattern(2, 2, 0);
            vAddNumToPattern(0, 6, 0);
            vAddNumToPattern(_date.u8Year / 10, 2, 6);
            vAddNumToPattern(_date.u8Year % 10, 6, 6);
            //add dot in front of last 2 digits of year
            vSetInPattern(0, 8, 1);
            break;
    }
}

#define TOGGLE(reg, bit) (((reg) & ~(bit)) | (((reg) ^ (bit)) & (bit)))

void eHandleButton(TE_BUTTONS eButton) {
    if (_eCurMenuState != _aeMenuStateChanges[_eCurMenuState][eButton]) {
        leaveState(_eCurMenuState);
        _eCurMenuState = _aeMenuStateChanges[_eCurMenuState][eButton];
        enterState(_eCurMenuState);
    } else {
        //the button is handled in this state
        switch (_eCurMenuState) {
            case eMENU_SET_CONFIG_ES_IST:
                _config = TOGGLE(_config, eCONF_ES_IST);
                _configChanged = TRUE;
                vClearPattern();
                if (_config & eCONF_ES_IST) {
                    vAddTextToPattern(eTEXT_ES_IST);
                }
                break;

            case eMENU_SET_CONFIG_MITTERNACHT:
                _config = TOGGLE(_config, eCONF_MITTERNACHT);
                _configChanged = TRUE;
                vClearPattern();
                if (_config & eCONF_MITTERNACHT) {
                    vAddTextToPattern(eTEXT_MITTERNACHT);
                }
                break;

            case eMENU_SET_CONFIG_NULL:
                _config = TOGGLE(_config, eCONF_NULL);
                _configChanged = TRUE;
                vClearPattern();
                if (_config & eCONF_NULL) {
                    vAddTextToPattern(eTEXT_NULL_H);
                    vAddTextToPattern(eTEXT_NULL_UHR);
                }
                break;

            case eMENU_SET_CONFIG_VIERTEL_VOR_NACH:
                _config = TOGGLE(_config, eCONF_VIERTEL_VOR_NACH);
                _configChanged = TRUE;
                vClearPattern();
                if (_config & eCONF_VIERTEL_VOR_NACH) {
                    vAddTextToPattern(eTEXT_VIERTEL);
                    vAddTextToPattern(eTEXT_VOR);
                    vAddTextToPattern(eTEXT_NACH);
                } else {
                    vAddTextToPattern(eTEXT_DREIVIERTEL);
                }
                break;

            case eMENU_SET_CONFIG_ZWANZIG_VOR_NACH:
                _config = TOGGLE(_config, eCONF_ZWANZIG_VOR_NACH);
                _configChanged = TRUE;
                vClearPattern();
                if (_config & eCONF_ZWANZIG_VOR_NACH) {
                    vAddTextToPattern(eTEXT_ZWANZIG);
                    vAddTextToPattern(eTEXT_VOR);
                    vAddTextToPattern(eTEXT_NACH);
                }
                break;

            case eMENU_SET_DAY:
                //increment day
                _date.u8Day++;

                if (_date.u8Day == 32) {
                    _date.u8Day = 1;
                }

                _dateChanged = TRUE;

                vAddNumToPattern(_date.u8Day / 10, 2, 0);
                vAddNumToPattern(_date.u8Day % 10, 6, 0);

                break;

            case eMENU_SET_MONTH:
                //increment month
                _date.u8Month++;

                if (_date.u8Month == 13) {
                    _date.u8Month = 1;
                }

                _dateChanged = TRUE;

                vAddNumToPattern(_date.u8Month / 10, 2, 6);
                vAddNumToPattern(_date.u8Month % 10, 6, 6);
                break;

            case eMENU_SET_YEAR:
                //increment year
                _date.u8Year++;

                if (_date.u8Year == 30) {
                    _date.u8Year = 16;
                }

                _dateChanged = TRUE;

                vAddNumToPattern(_date.u8Year / 10, 2, 6);
                vAddNumToPattern(_date.u8Year % 10, 6, 6);
                break;

            case eMENU_SET_HOUR:
                //increment hour
                _time.u8Hour++;

                if (_time.u8Hour == 24) {
                    _time.u8Hour = 0;
                }

                _timeChanged = TRUE;

                vAddNumToPattern(_time.u8Hour / 10, 2, 0);
                vAddNumToPattern(_time.u8Hour % 10, 6, 0);
                break;

            case eMENU_SET_MINUTE:
                //increment minute
                _time.u8Minute++;

                if (_time.u8Minute == 60) {
                    _time.u8Minute = 0;
                }

                _timeChanged = TRUE;

                vAddNumToPattern(_time.u8Minute / 10, 2, 6);
                vAddNumToPattern(_time.u8Minute % 10, 6, 6);
                break;

            case eMENU_MAIN_TEST_DISPLAY:
                //perform the display test and stay on current item
                vTestDisplay();
                break;

            case eMENU_MAIN_INVALIDATE_TIME:
                _timeInvalidate = TRUE;
                vSetInPattern(10, 10, 1);
                break;

            default:
                break;
        }
    }
}

inline TE_MENU_STATE menuGetState() {
    return _eCurMenuState;
}
