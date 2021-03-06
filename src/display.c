#include <string.h>
#include "common.h"
#include "config.h"
#include "display.h"

//defines
#define ROW_TIME_RES (125)
#define ROW_TIME (8 * ROW_TIME_RES) // 1s / 60fps / 12rows = 1389us -> use 1000us

static const U16 ROW_ON_TIME_0 = (1 * ROW_TIME_RES);
static const U16 ROW_ON_TIME_1 = (2 * ROW_TIME_RES);
static const U16 ROW_ON_TIME_2 = (3 * ROW_TIME_RES);
static const U16 ROW_ON_TIME_3 = (4 * ROW_TIME_RES);
static const U16 ROW_ON_TIME_4 = (5 * ROW_TIME_RES);
static const U16 ROW_ON_TIME_5 = (6 * ROW_TIME_RES);
static const U16 ROW_ON_TIME_6 = (7 * ROW_TIME_RES);
static const U16 ROW_ON_TIME_7 = (8 * ROW_TIME_RES);

//typedefs

typedef struct {
    const U8 u8Row;
    const U16 u16Cols;
} TS_POSITION;

//variables
static U16 _au16Pattern[PATTERN_SIZE];
static U8 _brightness = eCONF_MAX_BRIGHTNESS;

static const U16 _au16ColBit[16] = {
    0x8000u,
    0x4000u,
    0x2000u,
    0x1000u,
    0x0800u,
    0x0400u,
    0x0200u,
    0x0100u,
    0x0080u,
    0x0040u,
    0x0020u,
    0x0010u,
    0x0008u,
    0x0004u,
    0x0002u,
    0x0001u
};

static const U16 _au16ColClrBit[16] = {
    ~0x8000u,
    ~0x4000u,
    ~0x2000u,
    ~0x1000u,
    ~0x0800u,
    ~0x0400u,
    ~0x0200u,
    ~0x0100u,
    ~0x0080u,
    ~0x0040u,
    ~0x0020u,
    ~0x0010u,
    ~0x0008u,
    ~0x0004u,
    ~0x0002u,
    ~0x0001u
};

static const TS_POSITION _astPos[eTEXT_NUM_ENTRIES] = {
    //eES_IST
    {
        0,
        0b1101110000000000
    },
    //eNULL_H
    {
        0,
        0b0000000111100000
    },
    //eNULL_UHR (only UHR)
    {
        1,
        0b1110000000000000
    },
    //eFUENF
    {
        2,
        0b0111100000000000
    },
    //eZEHN
    {
        2,
        0b0000011110000000
    },
    //eF"UNFZEHN
    {
        2,
        0b0111111110000000
    },
    //eVIERTEL
    {
        3,
        0b0000111111100000
    },
    //eZWANZIG
    {
        1,
        0b0000111111100000
    },
    //eDREIVIERTEL
    {
        3,
        0b1111111111100000
    },
    //eVOR
    {
        4,
        0b0111000000000000
    },
    //eNACH
    {
        4,
        0b0000111100000000
    },
    //eHALB
    {
        5,
        0b1111000000000000
    },
    //eMITTERNACHT
    {
        6,
        0b1111111111100000
    },
    //eZWOELF_H
    {
        9,
        0b0000001111100000
    },
    //eEINS_H
    {
        5,
        0b0000000111100000
    },
    //eZWEI_H
    {
        5,
        0b0000011110000000
    },
    //eDREI_H
    {
        7,
        0b0111100000000000
    },
    //eVIER_H
    {
        8,
        0b1111000000000000
    },
    //eFUENF_H
    {
        9,
        0b0011110000000000
    },
    //eSECHS_H
    {
        7,
        0b0000011111000000
    },
    //eSIEBEN_H
    {
        8,
        0b0000111111000000
    },
    //eACHT_H
    {
        6,
        0b0000000111100000
    },
    //eNEUN_H
    {
        10,
        0b0001111000000000
    },
    //eZEHN_H
    {
        10,
        0b1111000000000000
    },
    //eELF_H
    {
        9,
        0b1110000000000000
    },
    //eEIN_H
    {
        5,
        0b0000000111000000
    },
    //eUHR
    {
        10,
        0b0000000011100000
    }
};

//all numbers are 3x5
static const U16 _au16Nums[10] = {
    //***
    //* *
    //* *
    //* *
    //***
    0b1111011011011110,
    //  *
    //  *
    //  *
    //  *
    //  *
    0b0010010010010010,
    //***
    //  *
    //***
    //*
    //***
    0b1110011111001110,
    //***
    //  *
    //***
    //  *
    //***
    0b1110011110011110,
    //* *
    //* *
    //***
    //  *
    //  *
    0b1011011110010010,
    //***
    //*
    //***
    //  *
    //***
    0b1111001110011110,
    //***
    //*
    //***
    //* *
    //***
    0b1111001111011110,
    //***
    //  *
    //  *
    //  *
    //  *
    0b1110010010010010,
    //***
    //* *
    //***
    //* *
    //***
    0b1111011111011110,
    //***
    //* *
    //***
    //  *
    //***
    0b1111011110011110
};

static void _vOnOffCol(unsigned char ucCol, U8 ucOn) {
    static BOOL colOn = FALSE;

    colOn = ucOn != FALSE;

    switch (ucCol) {
        case 0:
            COL0 = colOn;
            break;

        case 1:
            COL1 = colOn;
            break;

        case 2:
            COL2 = colOn;
            break;

        case 3:
            COL3 = colOn;
            break;

        case 4:
            COL4 = colOn;
            break;

        case 5:
            COL5 = colOn;
            break;

        case 6:
            COL6 = colOn;
            break;

        case 7:
            COL7 = colOn;
            break;

        case 8:
            COL8 = colOn;
            break;

        case 9:
            COL9 = colOn;
            break;

        case 10:
            COL10 = colOn;
            break;

        case 0xFF: // fall through
        default:
            COL0 = 0;
            COL1 = 0;
            COL2 = 0;
            COL3 = 0;
            COL4 = 0;
            COL5 = 0;
            COL6 = 0;
            COL7 = 0;
            COL8 = 0;
            COL9 = 0;
            COL10 = 0;
            break;
    }
}

static void _vOnOffRow(unsigned char ucRow, U16 ucOn) {
    static BOOL rowOn = FALSE;

    rowOn = ucOn != FALSE;

    //we have already disabled everything so we only need
    //to take action here if a row is to be enabled
    switch (ucRow) {
        case 0:
            ROW0 = rowOn;
            break;

        case 1:
            ROW1 = rowOn;
            break;

        case 2:
            ROW2 = rowOn;
            break;

        case 3:
            ROW3 = rowOn;
            break;

        case 4:
            ROW4 = rowOn;
            break;

        case 5:
            ROW5 = rowOn;
            break;

        case 6:
            ROW6 = rowOn;
            break;

        case 7:
            ROW7 = rowOn;
            break;

        case 8:
            ROW8 = rowOn;
            break;

        case 9:
            ROW9 = rowOn;
            break;

        case 10:
            ROW10 = rowOn;
            break;

        case 11:
            ROW11 = rowOn;
            break;

        case 0xFF: // fall through
        default:
            ROW0 = 0;
            ROW1 = 0;
            ROW2 = 0;
            ROW3 = 0;
            ROW4 = 0;
            ROW5 = 0;
            ROW6 = 0;
            ROW7 = 0;
            ROW8 = 0;
            ROW9 = 0;
            ROW10 = 0;
            ROW11 = 0;
            break;
    }
}

#define ROW_ON_TIME(brightness) (ROW_ON_TIME_##brightness)
#define ROW_OFF_TIME(brightness) (ROW_TIME - (ROW_ON_TIME_##brightness))

#define ROW_ON_CASE(brightness)              \
    case (brightness):                       \
        DELAY_US(ROW_ON_TIME(brightness)); \
        break;

#define ROW_OFF_CASE(brightness)                        \
    case (brightness):                                  \
        DELAY_US(ROW_OFF_TIME(brightness)); \
        break;

void vDelayRowOnByBrightness()
{
    switch (_brightness)
    {
        default: // fall-through
        ROW_ON_CASE(0);
        ROW_ON_CASE(1);
        ROW_ON_CASE(2);
        ROW_ON_CASE(3);
        ROW_ON_CASE(4);
        ROW_ON_CASE(5);
        ROW_ON_CASE(6);
        ROW_ON_CASE(7);
    }
}

void vDelayRowOffByBrightness()
{
    switch (_brightness)
    {
        default: // fall-through
        ROW_OFF_CASE(0);
        ROW_OFF_CASE(1);
        ROW_OFF_CASE(2);
        ROW_OFF_CASE(3);
        ROW_OFF_CASE(4);
        ROW_OFF_CASE(5);
        ROW_OFF_CASE(6);
        ROW_OFF_CASE(7);
    }

}

void vWriteTime(TS_TIME *pstTime, TE_CONFIG eeConfig) {
#define MAX_ITEMS 6
    U8 ucHour,
            ucMinute,
            aucTime[MAX_ITEMS],
            ucCurrentItem,
            ucI;
    TE_CONFIG config;

    ucHour = pstTime->u8Hour;

    if (ucHour > 23) {
        return;
    }

    ucMinute = pstTime->u8Minute;

    if (ucMinute > 59) {
        return;
    }

    config = configGet();

    ucCurrentItem = (U8)-1;

    if (config & eCONF_ES_IST) {
        aucTime[++ucCurrentItem] = eTEXT_ES_IST;
    }

    if (config & eCONF_MITTERNACHT && ucHour == 0 && ucMinute == 0) {
        aucTime[++ucCurrentItem] = eTEXT_MITTERNACHT;
    } else if (config & eCONF_NULL && ucHour == 0 && ucMinute < 25) {
        aucTime[++ucCurrentItem] = eTEXT_NULL_H;
        aucTime[++ucCurrentItem] = eTEXT_NULL_UHR;

        if (ucMinute < 5) {
            ;
        } else if (ucMinute < 10) {
            aucTime[++ucCurrentItem] = eTEXT_FUENF;
        } else if (ucMinute < 15) {
            aucTime[++ucCurrentItem] = eTEXT_ZEHN;
        } else if (ucMinute < 20) {
            aucTime[++ucCurrentItem] = eTEXT_FUENFZEHN;
        } else if (ucMinute < 25) {
            aucTime[++ucCurrentItem] = eTEXT_ZWANZIG;
        }
    } else {
        unsigned char ucNextHour;

        //convert 24 to 12 hours
        ucHour %= 12;

        if (ucHour >= 11) {
            ucNextHour = 0;
        } else {
            ucNextHour = ucHour + 1;
        }

        if (ucMinute < 5) {
            if (ucHour == 1) {
                aucTime[++ucCurrentItem] = eTEXT_EIN_H;
            } else {
                aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucHour;
            }
            aucTime[++ucCurrentItem] = eTEXT_UHR;
        } else if (ucMinute < 10) {
            aucTime[++ucCurrentItem] = eTEXT_FUENF;
            aucTime[++ucCurrentItem] = eTEXT_NACH;
            aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucHour;
        } else if (ucMinute < 15) {
            aucTime[++ucCurrentItem] = eTEXT_ZEHN;
            aucTime[++ucCurrentItem] = eTEXT_NACH;
            aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucHour;
        } else if (ucMinute < 20) {
            aucTime[++ucCurrentItem] = eTEXT_VIERTEL;
            if (config & eCONF_VIERTEL_VOR_NACH) {
                aucTime[++ucCurrentItem] = eTEXT_NACH;
                aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucHour;
            } else {
                aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucNextHour;
            }
        } else if (ucMinute < 25) {
            if (config & eCONF_ZWANZIG_VOR_NACH) {
                aucTime[++ucCurrentItem] = eTEXT_ZWANZIG;
                aucTime[++ucCurrentItem] = eTEXT_NACH;
                aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucHour;
            } else {
                aucTime[++ucCurrentItem] = eTEXT_ZEHN;
                aucTime[++ucCurrentItem] = eTEXT_VOR;
                aucTime[++ucCurrentItem] = eTEXT_HALB;
                aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucNextHour;
            }
        } else if (ucMinute < 30) {
            aucTime[++ucCurrentItem] = eTEXT_FUENF;
            aucTime[++ucCurrentItem] = eTEXT_VOR;
            aucTime[++ucCurrentItem] = eTEXT_HALB;
            aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucNextHour;
        } else if (ucMinute < 35) {
            aucTime[++ucCurrentItem] = eTEXT_HALB;
            aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucNextHour;
        } else if (ucMinute < 40) {
            aucTime[++ucCurrentItem] = eTEXT_FUENF;
            aucTime[++ucCurrentItem] = eTEXT_NACH;
            aucTime[++ucCurrentItem] = eTEXT_HALB;
            aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucNextHour;
        } else if (ucMinute < 45) {
            if (config & eCONF_ZWANZIG_VOR_NACH) {
                aucTime[++ucCurrentItem] = eTEXT_ZWANZIG;
                aucTime[++ucCurrentItem] = eTEXT_VOR;
            } else {
                aucTime[++ucCurrentItem] = eTEXT_ZEHN;
                aucTime[++ucCurrentItem] = eTEXT_NACH;
                aucTime[++ucCurrentItem] = eTEXT_HALB;
            }
            aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucNextHour;
        } else if (ucMinute < 50) {
            if (config & eCONF_VIERTEL_VOR_NACH) {
                aucTime[++ucCurrentItem] = eTEXT_VIERTEL;
                aucTime[++ucCurrentItem] = eTEXT_VOR;
            } else {
                aucTime[++ucCurrentItem] = eTEXT_DREIVIERTEL;
            }
            aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucNextHour;
        } else if (ucMinute < 55) {
            aucTime[++ucCurrentItem] = eTEXT_ZEHN;
            aucTime[++ucCurrentItem] = eTEXT_VOR;
            aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucNextHour;
        } else if (ucMinute < 60) {
            aucTime[++ucCurrentItem] = eTEXT_FUENF;
            aucTime[++ucCurrentItem] = eTEXT_VOR;
            aucTime[++ucCurrentItem] = eTEXT_ZWOELF_H + ucNextHour;
        }
    }

    //started counting at -1
    ++ucCurrentItem;

    //the last row is following later for minutes 1-4 (border LEDs)
    for (ucI = 0; ucI < ucCurrentItem; ++ucI) {
        U8 ucCol, ucRow;

        ucRow = _astPos[aucTime[ucI]].u8Row;

        for (ucCol = 0; ucCol < NUM_COLS; ++ucCol) {
            _vOnOffCol(ucCol, (_astPos[aucTime[ucI]].u16Cols & _au16ColBit[ucCol]) ? 1 : 0);
        }

        _vOnOffRow(ucRow, 1);
        vDelayRowOnByBrightness();
        _vOnOffRow(ucRow, 0);
        vDelayRowOffByBrightness();
    }

    _vOnOffCol(0xFF, 0u);

    //write 12th row here (minutes)
    switch (ucMinute % 5) {
        case 4:
            _vOnOffCol(NUM_COLS - 1, 1u);
            //fall through

        case 3:
            _vOnOffCol(NUM_COLS - 2, 1u);
            //fall through

        case 2:
            _vOnOffCol(0, 1u);
            //fall through

        case 1:
            _vOnOffCol(1, 1u);

            _vOnOffRow(NUM_ROWS - 1, 1u);
            vDelayRowOnByBrightness();
            _vOnOffRow(NUM_ROWS - 1, 0u);
            vDelayRowOffByBrightness();

            break;

        default:
            break;
    }

    //disable all columns
    _vOnOffCol(0xFF, 0u);
}

void vAddTextToPattern(TE_CLOCK_TEXT eText) {
    U8 u8Col;

    for (u8Col = 0; u8Col < NUM_COLS; ++u8Col) {
        if (_astPos[eText].u16Cols & _au16ColBit[u8Col]) {
            _au16Pattern[_astPos[eText].u8Row] |= _au16ColBit[u8Col];
        }
    }
}

void vRemoveTextFromPattern(TE_CLOCK_TEXT eText) {
    U8 u8Col;

    for (u8Col = 0; u8Col < NUM_COLS; ++u8Col) {
        if (_astPos[eText].u16Cols & _au16ColBit[u8Col]) {
            _au16Pattern[_astPos[eText].u8Row] &= _au16ColClrBit[u8Col];
        }
    }
}

void vTestDisplay(void) {
    U8 u8Row, u8Col;

    for (u8Row = 0; u8Row < NUM_ROWS; ++u8Row) {
        _vOnOffRow(u8Row, 1);
        for (u8Col = 0; u8Col < NUM_COLS; ++u8Col) {
            //switch current column on
            _vOnOffCol(u8Col, 1u);
            DELAY_MS(50);
            //switch current column off
            _vOnOffCol(u8Col, 0u);
        }
        _vOnOffRow(u8Row, 0u);
    }
}

void vClearPattern() {
    memset(_au16Pattern, 0, sizeof (_au16Pattern));
}

void vSetInPattern(U8 u8Col, U8 u8Row, U8 u8On) {
    if (u8On) {
        _au16Pattern[u8Row] |= _au16ColBit[u8Col];
    } else {
        _au16Pattern[u8Row] &= _au16ColClrBit[u8Col];
    }
}

/*
 * add number u8Num 0-9 to pattern with upper left corner u8Col, u8Row
 */
void vAddNumToPattern(U8 u8Num, U8 u8Col, U8 u8Row) {
    U8 u8CurRow, u8CurCol, u8Bit = 15;

    for (u8CurRow = 0; u8CurRow < 5; ++u8CurRow) {
        for (u8CurCol = 0; u8CurCol < 3; ++u8CurCol) {
            vSetInPattern(u8Col + u8CurCol, u8Row + u8CurRow, (_au16Nums[u8Num] >> u8Bit) & 0x1);
            --u8Bit;
        }
    }
}

void vWritePattern() {
    U8 u8Row, u8Col;

    for (u8Row = 0; u8Row < NUM_ROWS; ++u8Row) {
        if (!_au16Pattern[u8Row]) {
            continue;
        }

        for (u8Col = 0; u8Col < NUM_COLS; ++u8Col) {
            _vOnOffCol(u8Col, (_au16Pattern[u8Row] & _au16ColBit[u8Col]) ? 1u : 0u);
        }

        _vOnOffRow(u8Row, 1);
        vDelayRowOnByBrightness();
        _vOnOffRow(u8Row, 0);
        vDelayRowOffByBrightness();
    }

    _vOnOffCol(0xFF, 0u);
}

void vSetBrightness(const U8 brightness) {
    if (brightness > eCONF_MAX_BRIGHTNESS) {
        _brightness = eCONF_MAX_BRIGHTNESS;
    }
    else
    {
        _brightness = brightness;
    }
}
