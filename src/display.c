#include <string.h>
#include <htc.h>
#include "ledmatrix.h"
#include "display.h"

//variables
static const TS_POSITION _astPos[eNUM_ENTRIES] =
{
  //eES_IST
  {{0},  {0b11011100, 0b00000000}},
  //eFUENF
  {{1},  {0b11110000, 0b00000000}},
  //eZEHN
  {{0},  {0b00000001, 0b11100000}},
  //eVIERTEL
  {{2},  {0b00001111, 0b11100000}},
  //eZWANZIG
  {{1},  {0b00001111, 0b11100000}},
  //eDREIVIERTEL
  {{2},  {0b11111111, 0b11100000}},
  //eGENAU
  {{3},  {0b01111100, 0b00000000}},
  //eVOR
  {{4},  {0b01110000, 0b00000000}},
  //eNACH
  {{3},  {0b00000011, 0b11000000}},
  //eHALB
  {{4},  {0b00000001, 0b11100000}},
  //eZWOELF
  {{9},  {0b01111100, 0b00000000}},
  //eEINS
  {{6},  {0b00000111, 0b10000000}},
  //eZWEI
  {{6},  {0b00011110, 0b00000000}},
  //eDREI
  {{7},  {0b00000001, 0b11100000}},
  //eVIER
  {{5},  {0b11110000, 0b00000000}},
  //eFUENF_H
  {{9},  {0b00000111, 0b10000000}},
  //eSECHS
  {{7},  {0b01111100, 0b00000000}},
  //eSIEBEN
  {{5},  {0b00000111, 0b11100000}},
  //eACHT
  {{8},  {0b00000001, 0b11100000}},
  //eNEUN
  {{10}, {0b00011110, 0b00000000}},
  //eZEHN_H
  {{10}, {0b11110000, 0b00000000}},
  //eELF
  {{6},  {0b11100000, 0b00000000}},
  //eSEX
  {{6},  {0b00000000, 0b11100000}},
  //eMITTERNACHT
  {{8},  {0b11111111, 0b11100000}},
  //eUHR
  {{10}, {0b00000000, 0b11100000}}
};

static void _vOnOffCol(unsigned char ucCol, unsigned char ucOn)
{
  switch (ucCol)
  {
    case 0:
      COL0 = ucOn;
      break;

    case 1:
      COL1 = ucOn;
      break;

    case 2:
      COL2 = ucOn;
      break;

    case 3:
      COL3 = ucOn;
      break;

    case 4:
      COL4 = ucOn;
      break;

    case 5:
      COL5 = ucOn;
      break;

    case 6:
      COL6 = ucOn;
      break;

    case 7:
      COL7 = ucOn;
      break;

    case 8:
      COL8 = ucOn;
      break;

    case 9:
      COL9 = ucOn;
      break;

    case 10:
      COL10 = ucOn;
      break;

    case 0xFF:
      COL0  = ucOn;
      COL1  = ucOn;
      COL2  = ucOn;
      COL3  = ucOn;
      COL4  = ucOn;
      COL5  = ucOn;
      COL6  = ucOn;
      COL7  = ucOn;
      COL8  = ucOn;
      COL9  = ucOn;
      COL10 = ucOn;
      break;
    
    default:
      COL0  = 0;
      COL1  = 0;
      COL2  = 0;
      COL3  = 0;
      COL4  = 0;
      COL5  = 0;
      COL6  = 0;
      COL7  = 0;
      COL8  = 0;
      COL9  = 0;
      COL10 = 0;
      break;
  }
}

static void _vOnOffRow(unsigned char ucRow, unsigned char ucOn)
{
  //disable all rows first since we do not want
  //more then one row enabled at a time
  ROW0  = 0;
  ROW1  = 0;
  ROW2  = 0;
  ROW3  = 0;
  ROW4  = 0;
  ROW5  = 0;
  ROW6  = 0;
  ROW7  = 0;
  ROW8  = 0;
  ROW9  = 0;
  ROW10 = 0;
  ROW11 = 0;

  if (ucOn)
  {
    //we have already disabled everything so we only need
    //to take action here if a row is to be enabled
    switch (ucRow)
    {
      case 0:
      ROW0 = ucOn;
      break;

      case 1:
      ROW1 = ucOn;
      break;

      case 2:
      ROW2 = ucOn;
      break;

      case 3:
      ROW3 = ucOn;
      break;

      case 4:
      ROW4 = ucOn;
      break;

      case 5:
      ROW5 = ucOn;
      break;

      case 6:
      ROW6 = ucOn;
      break;

      case 7:
      ROW7 = ucOn;
      break;

      case 8:
      ROW8 = ucOn;
      break;

      case 9:
      ROW9 = ucOn;
      break;

      case 10:
      ROW10 = ucOn;
      break;

      case 11:
      ROW11 = ucOn;
      break;

      default:
      break;
    }
  }
}

void vWriteTime(unsigned short long u24TimeInSecs, unsigned char u8Ossi)
{
  #define MAX_ITEMS 6
  unsigned char ucRow,
                ucHour,
                ucMinute,
                aucTime[MAX_ITEMS],
                ucCurrentItem;

  memset(aucTime, 0xFF, sizeof(aucTime));

  ucHour = u24TimeInSecs / SECS_IN_HOUR;

  if (ucHour > 23)
  {
    return;
  }

  ucMinute = (u24TimeInSecs % SECS_IN_HOUR) / SECS_IN_MIN;

  if (ucMinute > 59)
  {
    return;
  }

  ucCurrentItem = 0;

  aucTime[ucCurrentItem++] = eES_IST;

  if (ucMinute < 5 &&
      ucHour == 0)
  {
    aucTime[ucCurrentItem++] = eMITTERNACHT;
  }
  else
  {
    unsigned char ucNextHour;
  
    //convert 24 to 12 hours
    ucHour %= 12;
    
    if (ucHour == 11)
    {
      ucNextHour = 0;
    }
    else
    {
      ucNextHour = ucHour + 1;
    }
    
    if (ucMinute < 5)
    {
      aucTime[ucCurrentItem++] = eZWOELF + ucHour;
      aucTime[ucCurrentItem++] = eUHR;
    }
    else if (ucMinute < 10)
    {
      aucTime[ucCurrentItem++] = eFUENF;
      aucTime[ucCurrentItem++] = eNACH;
      aucTime[ucCurrentItem++] = eZWOELF + ucHour;
    }
    else if (ucMinute < 15)
    {
      aucTime[ucCurrentItem++] = eZEHN;
      aucTime[ucCurrentItem++] = eNACH;
      aucTime[ucCurrentItem++] = eZWOELF + ucHour;
    }
    else if (ucMinute < 20)
    {
      aucTime[ucCurrentItem++] = eVIERTEL;

      if (u8Ossi)
      {
        aucTime[ucCurrentItem++] = eZWOELF + ucNextHour;
      }
      else
      {
        aucTime[ucCurrentItem++] = eNACH;
        aucTime[ucCurrentItem++] = eZWOELF + ucHour;
      }
    }
    else if (ucMinute < 25)
    {
      aucTime[ucCurrentItem++] = eZEHN;
      aucTime[ucCurrentItem++] = eVOR;
      aucTime[ucCurrentItem++] = eHALB;
      aucTime[ucCurrentItem++] = eZWOELF + ucNextHour;
    }
    else if (ucMinute < 30)
    {
      aucTime[ucCurrentItem++] = eFUENF;
      aucTime[ucCurrentItem++] = eVOR;
      aucTime[ucCurrentItem++] = eHALB;
      aucTime[ucCurrentItem++] = eZWOELF + ucNextHour;
    }
    else if (ucMinute < 35)
    {
      aucTime[ucCurrentItem++] = eHALB;
      aucTime[ucCurrentItem++] = eZWOELF + ucNextHour;
    }
    else if (ucMinute < 40)
    {
      aucTime[ucCurrentItem++] = eFUENF;
      aucTime[ucCurrentItem++] = eNACH;
      aucTime[ucCurrentItem++] = eHALB;
      aucTime[ucCurrentItem++] = eZWOELF + ucNextHour;
    }
    else if (ucMinute < 45)
    {
      aucTime[ucCurrentItem++] = eZEHN;
      aucTime[ucCurrentItem++] = eNACH;
      aucTime[ucCurrentItem++] = eHALB;
      aucTime[ucCurrentItem++] = eZWOELF + ucNextHour;
    }
    else if (ucMinute < 50)
    {
      if (u8Ossi)
      {
        aucTime[ucCurrentItem++] = eDREIVIERTEL;
      }
      else
      {
        aucTime[ucCurrentItem++] = eVIERTEL;
        aucTime[ucCurrentItem++] = eVOR;
      }

      aucTime[ucCurrentItem++] = eZWOELF + ucNextHour;
    }
    else if (ucMinute < 55)
    {
      aucTime[ucCurrentItem++] = eZEHN;
      aucTime[ucCurrentItem++] = eVOR;
      aucTime[ucCurrentItem++] = eZWOELF + ucNextHour;
    }
    else if (ucMinute < 60)
    {
      aucTime[ucCurrentItem++] = eFUENF;
      aucTime[ucCurrentItem++] = eVOR;
      aucTime[ucCurrentItem++] = eZWOELF + ucNextHour;
    }
  }

  //the last row is following later for minutes 1-4 (border LEDs)
  for (ucRow = 0; ucRow < (NUM_ROWS - 1); ucRow++)
  {
    ucCurrentItem = 0;
    
    _vOnOffCol(0xFF, 0);

    for (ucCurrentItem = 0; ucCurrentItem < MAX_ITEMS; ucCurrentItem++)
    {
      if (aucTime[ucCurrentItem] != 0xFF)
      {
        if (_astPos[aucTime[ucCurrentItem]].ucRow == ucRow)
        {
          unsigned char ucCol;
          
          for (ucCol = 0; ucCol < NUM_COLS; ucCol++)
          {
            CLRWDT();

            _vOnOffRow(0xFF, 0);

            if ((_astPos[aucTime[ucCurrentItem]].aucCols[ucCol / 8] >> (7 - (ucCol % 8))) & 0b1)
            {
              _vOnOffCol(ucCol, 1);
            }
          }
        }
      }
    }

    _vOnOffRow(ucRow, 1);
  }
  
  _vOnOffRow(0xFF, 0);

  //write 12th row here (minutes)
  switch (ucMinute % 5)
  {
    case 4:
    _vOnOffCol(NUM_COLS - 1, 1);
    //fall through
    case 3:
    _vOnOffCol(1, 1);
    //fall through
    case 2:
    _vOnOffCol(NUM_COLS - 2, 1);
    //fall through
    case 1:
    _vOnOffCol(0, 1);
    break;
  }

  _vOnOffRow(NUM_ROWS - 1, 1);

  //disable everything
  _vOnOffRow(0xFF, 0);
  _vOnOffCol(0xFF, 0);
}

void vTestDisplay(void)
{
  _vOnOffCol(0, 1);
  _vOnOffCol(1, 1);
  _vOnOffCol(2, 1);
  _vOnOffCol(3, 1);
  _vOnOffCol(4, 1);
  _vOnOffCol(5, 1);
  _vOnOffCol(6, 1);
  _vOnOffCol(7, 1);
  _vOnOffCol(8, 1);
  _vOnOffCol(9, 1);
  _vOnOffCol(10, 1);
  _vOnOffRow(0, 1);
  _vOnOffRow(1, 1);
  _vOnOffRow(2, 1);
  _vOnOffRow(3, 1);
  _vOnOffRow(4, 1);
  _vOnOffRow(5, 1);
  _vOnOffRow(6, 1);
  _vOnOffRow(7, 1);
  _vOnOffRow(8, 1);
  _vOnOffRow(9, 1);
  _vOnOffRow(10, 1);
  _vOnOffRow(0xFF, 0);
}

void vWritePattern(unsigned char *pu8Pattern)
{
  unsigned char u8Row, u8Col, u8Pos = 0, u8BitPos = 0;

  for (u8Row = 0; u8Row < NUM_ROWS; u8Row++)
  {
    for (u8Col = 0; u8Col < NUM_COLS; u8Col++)
    {
      _vOnOffCol(u8Col, pu8Pattern[u8Pos] & (1 << u8BitPos) ? 1 : 0);

      u8BitPos++;

      if (u8BitPos == 0x8)
      {
        u8Pos++;
        u8BitPos = 0;
      }
    }

    _vOnOffRow(u8Row, 1);
    DELAY_US(40);
    _vOnOffRow(u8Row, 0);
  }
}
