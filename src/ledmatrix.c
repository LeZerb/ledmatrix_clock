#include <string.h>
#include <htc.h>
#include "ledmatrix.h"
#include "display.h"

/*
** in-cicuit debugging disabled
** low voltage programming disabled
** fail-safe clock monitor for external clock disabled
** internal external clock switchover disabled
** brown-out reset enabled
** data unprotect
** code unprotect
** MCLR is enabled
** power-up timer enabled
** watchdog timer disabled - will be enabled in software
** use internal clock - CLKIN/CLKOUT are I/O
*/
__CONFIG(DEBUGDIS & LVPDIS & FCMDIS & IESODIS & BOREN & DUNPROTECT & UNPROTECT & MCLRDIS & PWRTDIS & WDTDIS & INTIO);

/*
** flash write protection is not enabled
** brown-out reset on 2.1V
*/
__CONFIG(BORV21);

//variables
static volatile unsigned short long int _u24SecondsToday = 0;

static volatile unsigned char _u8DCFBitsRecvd = 0;
static volatile unsigned char _u8DCFSyncDone  = 0;

static volatile unsigned char _u8ValidTime = 0;

//we need room for 2 complete DCF data sets
static volatile unsigned char _au8DCFData[((2 * 59) % 8) ? (((2 * 59) / 8) + 1) : ((2 * 59) / 8)];

//static unsigned char _au8Pattern[((NUM_COLS * NUM_ROWS) % 8) ? (((NUM_COLS * NUM_ROWS) / 8) + 1) : ((NUM_COLS * NUM_ROWS) / 8)];
static unsigned char _au8Pattern[(NUM_COLS % 8) ? ((NUM_COLS / 8) + 1) * NUM_ROWS : (NUM_COLS / 8) * NUM_ROWS];

//all numbers are 3x5
static const unsigned char _aau8Nums[10][2] =
{
  //***
  //* *
  //* *
  //* *
  //***
  {{0b11110110}, {0b11011110}},
  //  *
  //  *
  //  *
  //  *
  //  *
  {{0b00100100}, {0b10010010}},
  //***
  //  *
  //***
  //*
  //***
  {{0b11100111}, {0b11001110}},
  //***
  //  *
  //***
  //  *
  //***
  {{0b11100111}, {0b10011110}},
  //* *
  //* *
  //***
  //  *
  //  *
  {{0b10110111}, {0b10010010}},
  //***
  //*
  //***
  //  *
  //***
  {{0b11110011}, {0b10011110}},
  //***
  //*
  //***
  //* *
  //***
  {{0b11110011}, {0b11011110}},
  //***
  //  *
  //  *
  //  *
  //  *
  {{0b11100100}, {0b10010010}},
  //***
  //* *
  //***
  //* *
  //***
  {{0b11110111}, {0b11011110}},
  //***
  //* *
  //***
  //  *
  //***
  {{0b11110111}, {0b10011110}}
};

void /*interrupt*/ _vInterrupt(void)
{
  static unsigned short int _u16ClockITR = 0;

  if (T0IE && T0IF)
  {
    //we have an interrupt on T0
    T0IF = 0;

    _u16ClockITR++;

    if (!(_u16ClockITR % WRAPS_A_SEC))
    {
      if (_u16ClockITR >= (WRAPS_A_SEC * ITR_RESET_AFTER))
      {
        //DCF need to use this in order to find sync
        //therefore _u16ClockITR needs to run longer than 2 seconds (in this case ITR_RESET_AFTER)
        _u16ClockITR = 0;
      }

      _u24SecondsToday++;

      if (_u24SecondsToday >= SECS_A_DAY)
      {
        //a new day has come
        _u24SecondsToday = 0;
      }
    }
  }

  if (RBIE && RBIF)
  {
    static bit                _bRb2 = 0;

    //we need to read from RB0 before disabling the
    //interrupt since it would otherwise be raised continually
    PORTB;
    RBIF = 0;

    if (_bRb2 != RB2)
    {
      static unsigned short int _u16LastTime = 0;
      unsigned short int        u16TimeSince;

      _bRb2 = RB2;

      if (_u16ClockITR >= _u16LastTime)
      {
        u16TimeSince = _u16ClockITR - _u16LastTime;
      }
      else
      {
        u16TimeSince = _u16ClockITR + (WRAPS_A_SEC * ITR_RESET_AFTER) - _u16LastTime;
      }

      _u16LastTime = _u16ClockITR;

      if (_bRb2 == 0)
      {
        //falling edge since we are using an inverted output
        if ((u16TimeSince > (WRAPS_A_SEC * 3 / 50)) &&
            (u16TimeSince < (WRAPS_A_SEC * 7 / 50)))
        {
          //about 100ms is a '0'
          if (_u8DCFBitsRecvd < (2 * 59))
          {
            BIT_CLR(_au8DCFData[_u8DCFBitsRecvd / 8], _u8DCFBitsRecvd % 8);
          }

          _u8DCFBitsRecvd++;
        }
        else if ((u16TimeSince > (WRAPS_A_SEC *  8 / 50)) &&
                 (u16TimeSince < (WRAPS_A_SEC * 12 / 50)))
        {
          //about 200ms is a '1'
          if (_u8DCFBitsRecvd < (2 * 59))
          {
            BIT_SET(_au8DCFData[_u8DCFBitsRecvd / 8], _u8DCFBitsRecvd % 8);
          }

          _u8DCFBitsRecvd++;
        }
        else
        {
          //invalid interval length
          _u8DCFSyncDone  = 0;
          _u8DCFBitsRecvd = 0;
        }

        if (_u8DCFBitsRecvd > 59 &&
            !_u8DCFSyncDone)
        {
          //something went wrong since there would have had to be a sync before byte 60
          _u8DCFBitsRecvd = 0;
        }
      }
      else
      {
        //rising edge
        if (u16TimeSince > (3 * WRAPS_A_SEC / 2) &&
            u16TimeSince < (5 * WRAPS_A_SEC / 2))
        {
          _u8DCFSyncDone  = 1;

          if (_u8DCFBitsRecvd == 59)
          {
            //we have a complete data set available
            //if we do not yet have a time set use the time we have found

            if (!_u8ValidTime)
            {
              _u24SecondsToday = (//first calculate minutes
                                  (BIT(_au8DCFData, 21) *  1) +
                                  (BIT(_au8DCFData, 22) *  2) +
                                  (BIT(_au8DCFData, 23) *  4) +
                                  (BIT(_au8DCFData, 24) *  8) +
                                  (BIT(_au8DCFData, 25) * 10) +
                                  (BIT(_au8DCFData, 26) * 20) +
                                  (BIT(_au8DCFData, 27) * 40)) * 60 +
                                 (//and add hours
                                  (BIT(_au8DCFData, 29) *  1) +
                                  (BIT(_au8DCFData, 30) *  2) +
                                  (BIT(_au8DCFData, 31) *  4) +
                                  (BIT(_au8DCFData, 32) *  8) +
                                  (BIT(_au8DCFData, 33) * 10) +
                                  (BIT(_au8DCFData, 34) * 20)) * 60 * 60;

              _u8ValidTime = 1;
            }
          }
          else if (_u8DCFBitsRecvd == 2 * 59)
          {
            //sync is now done and we have two sets of data we can compare and set the time
            #warning !!!!!! needs implementing
          }
          else
          {
            _u8DCFBitsRecvd = 0;
          }
        }
        else if (u16TimeSince >= (5 * WRAPS_A_SEC / 2))
        {
          //should not happen
          _u8DCFSyncDone  = 0;
          _u8DCFBitsRecvd = 0;
        }
      }
    }
  }

  if (ADIE && ADIF)
  {
    //disable A/D interrupt and A/D
    ADIF = 0;
    ADON = 0;
  }
}

static void _vInit()
{
  //enable watchdog timer with prescaler 1:65536
  WDTPS3 = 1;
  WDTPS2 = 0;
  WDTPS1 = 1;
  WDTPS0 = 1;
  SWDTEN = 1;

  //disable port B pull-ups
  RBPU = 1;

  //timer 0 clock source is internal clock Fosc/4
  T0CS = 0;

  //prescaler is assigned to timer 0
  PSA = 0;

  //timer prescaler set to 1/32
  PS2 = 1;
  PS1 = 0;
  PS0 = 0;

  //8MHz
  IRCF2 = 1;
  IRCF1 = 1;
  IRCF1 = 1;
  
  //globally enable interrupts
  GIE = 1;
  
  //enable timer 0 wrap arount interrupt
  T0IE = 1;
  
  //disable analog inputs
  ANSEL  = 0;
  ANSELH = 0;
  
  //enable A/D on RB1/AN10
  //ANS10 = 1;
  
  //enable peripheral + A/D interrupt
  ADIF = 0;
  ADIE = 1;
  PEIE = 1;
  
  //configure A/D
  //use FRC
  ADCS1 = 1;
  ADCS0 = 1;
  //use RB1/AN10
  CHS3  = 1;
  CHS2  = 0;
  CHS1  = 1;
  CHS0  = 0;
  //right justified 2 bits in ADRESH 8 bits in ADRESL
  ADFM  = 1;
  //voltage references are Vss and Vdd
  VCFG1 = 0;
  VCFG0 = 0;
  
  PORTA = 0;
  PORTB = 0;
  PORTC = 0;
  PORTD = 0;
  PORTE = 0;
  
  //configure all ports as output
  TRISA = 0;
  TRISB = 0;
  TRISC = 0;
  TRISD = 0;
  TRISE = 0;
  
  //configure RB2 as input for DCF signal
  TRISB2 = 1;

  //enable interrupt on port RB2
  IOCB2 = 1;

  //enable port B interrupt
  RBIE  = 1;
}

void main(void)
{
  _vInit();

/*
  ADON = 1;
  //instead of delay we might want to repaint the display
  DELAY_MS(40);
  GODONE = 1;
*/

  //enable DCF module since we do not have a valid time on startup
  RC3 = 1;

  while (1)
  {
    //clear the watchdog timer
    CLRWDT();
#if 0
    if (!_u8ValidTime)
    {
      vWriteTime(_u24SecondsToday, 1);
    }
    else
    {
      unsigned char u8DCFBits    = _u8DCFBitsRecvd,
                    u8CurrentBit = 0;

      memset(_au8Pattern, 0, sizeof(_au8Pattern));

      while (u8CurrentBit < u8DCFBits)
      {
        if ((_au8DCFData[u8CurrentBit / 8] >> (u8CurrentBit % 8)) & 0x1)
        {
          BIT_SET(_au8Pattern[u8CurrentBit / 8], u8CurrentBit % 8);
        }

        u8CurrentBit++;
      }

      vWritePattern(_au8Pattern);
    }
#else
  memset(_au8Pattern, 0xFF, sizeof(_au8Pattern));
  vWritePattern(_au8Pattern);
#endif
  }
}
