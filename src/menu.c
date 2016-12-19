#include "common.h"
#include "config.h"
#include "display.h"
#include "menu.h"
#include "snake.h"

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
        eMENU_MAIN_TIME,
        eMENU_SHOW_TIME
    },
    //eMENU_MAIN_CONFIG
    {
        eMENU_MAIN_INVALIDATE_TIME,
        eMENU_SET_CONFIG_BRIGHTNESS
    },
    //eMENU_MAIN_TIME
    {
        eMENU_MAIN_CONFIG,
        eMENU_SET_HOUR
    },
    //eMENU_MAIN_INVALIDATE_TIME
    {
        eMENU_MAIN_TEST_DISPLAY,
        eMENU_MAIN_INVALIDATE_TIME
    },
    //eMENU_MAIN_TEST_DISPLAY
    {
        eMENU_MAIN_SNAKE,
        eMENU_MAIN_TEST_DISPLAY
    },
    //eMENU_MAIN_SNAKE
    {
        eMENU_NIRVANA,
        eMENU_SNAKE
    },
    //eMENU_SET_CONFIG_BRIGHTNESS
    {
        eMENU_SET_CONFIG_BRIGHTNESS,
        eMENU_SET_CONFIG_ES_IST
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
    //eMENU_SNAKE
    {
        eMENU_SNAKE,
        eMENU_SNAKE
    }
};

static TE_MENU_STATE _eCurMenuState = eMENU_NIRVANA;
static TS_TIME _oldTime = {0, 0, 0};
static TS_TIME _time = {0, 0, 0};
static TS_DATE _oldDate = {1, 1, 16};
static TS_DATE _date = {1, 1, 16};
static U8 _brightness = eCONF_BRIGHTNESS >> eCONF_BRIGHTNESS_SHIFT;
static TE_CONFIG _oldConfig = 0;
static TE_CONFIG _config = 0;
static BOOL _timeInvalidate = FALSE;

//implementation

#define ROW_TEST_DISPLAY 7

static void enterMainTestDisplay() {
    vClearPattern();
    vSetInPattern(0, ROW_TEST_DISPLAY, 1);
}

#define ROW_TEST_INVALIDATE_TIME 8

static void enterMainInvalidateTime() {
    vClearPattern();
    vSetInPattern(0, ROW_TEST_INVALIDATE_TIME, 1);
}

#define ROW_CONFIG 9

static void enterMainConfig() {
    vClearPattern();
    vSetInPattern(0, ROW_CONFIG, 1);
}

#define ROW_SET_TIME 10

static void enterMainTime() {
    vClearPattern();
    vSetInPattern(0, ROW_SET_TIME, 1);
    //show time 00:00
    vAddNumToPattern(_time.u8Hour / 10, 2, 0);
    vAddNumToPattern(_time.u8Hour % 10, 6, 0);
    vAddNumToPattern(_time.u8Minute / 10, 2, 6);
    vAddNumToPattern(_time.u8Minute % 10, 6, 6);
    //add colon
    vSetInPattern(10, 4, 1);
    vSetInPattern(10, 6, 1);
}

static void enterDate() {
    enterMainTime();
    //show date
    vAddNumToPattern(_date.u8Day / 10, 2, 0);
    vAddNumToPattern(_date.u8Day % 10, 6, 0);
    vAddNumToPattern(_date.u8Month / 10, 2, 6);
    vAddNumToPattern(_date.u8Month % 10, 6, 6);
    //remove colon
    vSetInPattern(10, 4, 0);
    vSetInPattern(10, 6, 0);
}

static void enterYear() {
    enterMainTime();
    //show year 20xx
    vAddNumToPattern(2, 2, 0);
    vAddNumToPattern(0, 6, 0);
    vAddNumToPattern(_date.u8Year / 10, 2, 6);
    vAddNumToPattern(_date.u8Year % 10, 6, 6);
    //remove colon
    vSetInPattern(10, 4, 0);
    vSetInPattern(10, 6, 0);
}

static void updateHour() {
    vAddNumToPattern(_time.u8Hour / 10, 2, 0);
    vAddNumToPattern(_time.u8Hour % 10, 6, 0);
}

static void updateMinute() {
    vAddNumToPattern(_time.u8Minute / 10, 2, 6);
    vAddNumToPattern(_time.u8Minute % 10, 6, 6);
}

static void updateDay() {
    vAddNumToPattern(_date.u8Day / 10, 2, 0);
    vAddNumToPattern(_date.u8Day % 10, 6, 0);
}

static void updateMonth() {
    vAddNumToPattern(_date.u8Month / 10, 2, 6);
    vAddNumToPattern(_date.u8Month % 10, 6, 6);
}

static void updateYear() {
    vAddNumToPattern(_date.u8Year / 10, 2, 6);
    vAddNumToPattern(_date.u8Year % 10, 6, 6);
}

static void updateConfigBrightness() {
    vSetBrightness((_config & eCONF_BRIGHTNESS) >> eCONF_BRIGHTNESS_SHIFT);
}

static void updateConfigEsIst() {
    if (_config & eCONF_ES_IST) {
        vAddTextToPattern(eTEXT_ES_IST);
    } else {
        vRemoveTextFromPattern(eTEXT_ES_IST);
    }
}

static void updateConfigMitternacht() {
    if (_config & eCONF_MITTERNACHT) {
        vAddTextToPattern(eTEXT_MITTERNACHT);
    } else {
        vRemoveTextFromPattern(eTEXT_MITTERNACHT);
    }
}

static void updateConfigNull() {
    if (_config & eCONF_NULL) {
        vAddTextToPattern(eTEXT_NULL_H);
        vAddTextToPattern(eTEXT_NULL_UHR);
    } else {
        vRemoveTextFromPattern(eTEXT_NULL_H);
        vRemoveTextFromPattern(eTEXT_NULL_UHR);
    }
}

static void updateConfigViertelVor() {
    if (_config & eCONF_VIERTEL_VOR_NACH) {
        vRemoveTextFromPattern(eTEXT_DREIVIERTEL);
        vAddTextToPattern(eTEXT_VIERTEL);
        vAddTextToPattern(eTEXT_VOR);
        vAddTextToPattern(eTEXT_NACH);
    } else {
        vRemoveTextFromPattern(eTEXT_VIERTEL);
        vRemoveTextFromPattern(eTEXT_VOR);
        vRemoveTextFromPattern(eTEXT_NACH);
        vAddTextToPattern(eTEXT_DREIVIERTEL);
    }
}

static void updateConfigZwanzigVor() {
    if (_config & eCONF_ZWANZIG_VOR_NACH) {
        vAddTextToPattern(eTEXT_ZWANZIG);
        vAddTextToPattern(eTEXT_VOR);
        vAddTextToPattern(eTEXT_NACH);
    } else {
        vRemoveTextFromPattern(eTEXT_ZWANZIG);
        vRemoveTextFromPattern(eTEXT_VOR);
        vRemoveTextFromPattern(eTEXT_NACH);
    }
}

static void enterState(TE_MENU_STATE state) {
    switch (state) {
        case eMENU_MAIN_CONFIG:
            enterMainConfig();
            updateConfigBrightness();
            updateConfigEsIst();
            updateConfigMitternacht();
            updateConfigNull();
            updateConfigViertelVor();
            updateConfigZwanzigVor();
            break;
        case eMENU_MAIN_INVALIDATE_TIME:
            enterMainInvalidateTime();
            break;
        case eMENU_MAIN_TEST_DISPLAY:
            enterMainTestDisplay();
            break;
        case eMENU_MAIN_TIME:
            enterMainTime();
            break;
        case eMENU_MAIN_SNAKE:
            snakeInit();
            break;
        case eMENU_SET_CONFIG_BRIGHTNESS:
            enterMainConfig();
            updateConfigEsIst();
            updateConfigMitternacht();
            updateConfigNull();
            updateConfigViertelVor();
            updateConfigZwanzigVor();
            vSetInPattern(1, ROW_CONFIG, 1);
            break;
        case eMENU_SET_CONFIG_ES_IST:
            enterMainConfig();
            vSetInPattern(1, ROW_CONFIG, 1);
            vSetInPattern(2, ROW_CONFIG, 1);
            updateConfigEsIst();
            break;
        case eMENU_SET_CONFIG_MITTERNACHT:
            enterMainConfig();
            vSetInPattern(1, ROW_CONFIG, 1);
            vSetInPattern(2, ROW_CONFIG, 1);
            vSetInPattern(3, ROW_CONFIG, 1);
            vSetInPattern(4, ROW_CONFIG, 1);
            vSetInPattern(5, ROW_CONFIG, 1);
            updateConfigMitternacht();
            break;
        case eMENU_SET_CONFIG_NULL:
            enterMainConfig();
            vSetInPattern(1, ROW_CONFIG, 1);
            vSetInPattern(2, ROW_CONFIG, 1);
            vSetInPattern(3, ROW_CONFIG, 1);
            vSetInPattern(4, ROW_CONFIG, 1);
            vSetInPattern(5, ROW_CONFIG, 1);
            vSetInPattern(6, ROW_CONFIG, 1);
            updateConfigNull();
            break;
        case eMENU_SET_CONFIG_VIERTEL_VOR_NACH:
            enterMainConfig();
            vSetInPattern(1, ROW_CONFIG, 1);
            vSetInPattern(2, ROW_CONFIG, 1);
            vSetInPattern(3, ROW_CONFIG, 1);
            updateConfigViertelVor();
            break;
        case eMENU_SET_CONFIG_ZWANZIG_VOR_NACH:
            enterMainConfig();
            vSetInPattern(1, ROW_CONFIG, 1);
            vSetInPattern(2, ROW_CONFIG, 1);
            vSetInPattern(3, ROW_CONFIG, 1);
            vSetInPattern(4, ROW_CONFIG, 1);
            updateConfigZwanzigVor();
            break;
        case eMENU_SET_DAY:
            enterDate();
            //add dot in front of day
            vSetInPattern(0, 2, 1);
            break;
        case eMENU_SET_HOUR:
            enterMainTime();
            //add dot in front of hour
            vSetInPattern(0, 2, 1);
            break;
        case eMENU_SET_MINUTE:
            enterMainTime();
            //add dot in front of minute
            vSetInPattern(0, 8, 1);
            break;
        case eMENU_SET_MONTH:
            enterDate();
            //add dot in front of month
            vSetInPattern(0, 8, 1);
            break;
        case eMENU_SET_YEAR:
            enterYear();
            //add dot in front of year
            vSetInPattern(0, 8, 1);
            break;
    }
}

static void leaveState(TE_MENU_STATE state, TE_MENU_STATE newState) {
    //are we changing to a new main menu state
    if (newState >= eMENU_MAIN_FIRST && newState <= eMENU_MAIN_LAST) {
        switch (state) {
            case eMENU_NIRVANA:
                _config = configGet();
                _oldConfig = _config;
                _brightness = (_oldConfig & eCONF_BRIGHTNESS) >> eCONF_BRIGHTNESS_SHIFT;
                dateGet(&_date);
                memcpy(&_oldDate, &_date, sizeof (_oldDate));
                timeGet(&_time);
                memcpy(&_oldTime, &_time, sizeof (_oldTime));
                _timeInvalidate = FALSE;
                break;
            case eMENU_MAIN_CONFIG:
                if (_config != _oldConfig) {
                    configSave(_config);
                    _oldConfig = _config;
                }
                break;
            case eMENU_MAIN_TIME:
                if (memcmp(&_time, &_oldTime, sizeof (_time))) {
                    timeSet(&_time);
                }
                if (memcmp(&_date, &_oldDate, sizeof (_date))) {
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
}

#define TOGGLE(reg, bit) (((reg) & ~(bit)) | (((reg) ^ (bit)) & (bit)))

void eHandleButton(TE_BUTTONS eButton) {
    if (_eCurMenuState != _aeMenuStateChanges[_eCurMenuState][eButton]) {
        leaveState(_eCurMenuState, _aeMenuStateChanges[_eCurMenuState][eButton]);
        _eCurMenuState = _aeMenuStateChanges[_eCurMenuState][eButton];
        enterState(_eCurMenuState);
    } else {
        //the button is handled in this state
        switch (_eCurMenuState) {
            case eMENU_SET_CONFIG_BRIGHTNESS:
                _brightness++;
                if (_brightness > (eCONF_BRIGHTNESS >> eCONF_BRIGHTNESS_SHIFT)) {
                    _brightness = 0;
                }
                _config &= ~eCONF_BRIGHTNESS;
                _config |= _brightness << eCONF_BRIGHTNESS_SHIFT;
                updateConfigBrightness();
                break;

            case eMENU_SET_CONFIG_ES_IST:
                _config = TOGGLE(_config, eCONF_ES_IST);
                updateConfigEsIst();
                break;

            case eMENU_SET_CONFIG_MITTERNACHT:
                _config = TOGGLE(_config, eCONF_MITTERNACHT);
                updateConfigMitternacht();
                break;

            case eMENU_SET_CONFIG_NULL:
                _config = TOGGLE(_config, eCONF_NULL);
                updateConfigNull();
                break;

            case eMENU_SET_CONFIG_VIERTEL_VOR_NACH:
                _config = TOGGLE(_config, eCONF_VIERTEL_VOR_NACH);
                updateConfigViertelVor();
                break;

            case eMENU_SET_CONFIG_ZWANZIG_VOR_NACH:
                _config = TOGGLE(_config, eCONF_ZWANZIG_VOR_NACH);
                updateConfigZwanzigVor();
                break;

            case eMENU_SET_DAY:
                //increment day
                _date.u8Day++;
                if (_date.u8Day == 32) {
                    _date.u8Day = 1;
                }
                updateDay();
                break;

            case eMENU_SET_MONTH:
                //increment month
                _date.u8Month++;
                if (_date.u8Month == 13) {
                    _date.u8Month = 1;
                }
                updateMonth();
                break;

            case eMENU_SET_YEAR:
                //increment year
                _date.u8Year++;
                if (_date.u8Year == 30) {
                    _date.u8Year = 16;
                }
                updateYear();
                break;

            case eMENU_SET_HOUR:
                //increment hour
                _time.u8Hour++;
                if (_time.u8Hour == 24) {
                    _time.u8Hour = 0;
                }
                updateHour();
                break;

            case eMENU_SET_MINUTE:
                //increment minute
                _time.u8Minute++;
                if (_time.u8Minute == 60) {
                    _time.u8Minute = 0;
                }
                updateMinute();
                break;

            case eMENU_MAIN_TEST_DISPLAY:
                //perform the display test and stay on current item
                vTestDisplay();
                break;

            case eMENU_MAIN_INVALIDATE_TIME:
                if (_timeInvalidate) {
                    _timeInvalidate = FALSE;
                } else {
                    _timeInvalidate = TRUE;
                }
                vSetInPattern(10, 10, _timeInvalidate);
                break;

            case eMENU_SNAKE:
                if (snakeRun(eButton)) {
                    leaveState(_eCurMenuState, eMENU_MAIN_SNAKE);
                    _eCurMenuState = eMENU_MAIN_SNAKE;
                    enterState(_eCurMenuState);
                }
                break;
        }
    }
}

inline TE_MENU_STATE menuGetState() {
    return _eCurMenuState;
}
