#include <string.h>
#include <htc.h>
#include "ledmatrix.h"
#include "display.h"

/*
** in-cicuit debugging disabled
** low voltage programming disabled
** fail-safe clock monitor for external clock enabled
** internal external clock switchover disabled
** brown-out reset enabled
** data unprotect
** code unprotect
** MCLR is enabled
** power-up timer enabled
** watchdog timer disabled - will be enabled in software
** use internal clock - CLKIN/CLKOUT are I/O
*/
__CONFIG(DEBUGDIS & LVPDIS & FCMEN & IESODIS & BOREN & DUNPROTECT & UNPROTECT & MCLRDIS & PWRTEN & WDTDIS & HS);

/*
** flash write protection is not enabled
** brown-out reset on 2.1V
*/
__CONFIG(BORV21);

//variables
static volatile unsigned long int _u24SecondsToday = 81000;//0;

static volatile unsigned char _u8DCFBitsRecvd = 0;
static volatile unsigned char _u8DCFSyncDone  = 0;

static volatile unsigned char _u8ValidTime = 0;

//we need room for 2 complete DCF data sets
static volatile unsigned char _au8DCFData[((2 * 59) / 8) + 1];

void interrupt _vInterrupt(void)
{
  static unsigned char _u8ClockITR = 0;

  if (T0IE && T0IF)
  {
    //we have an interrupt on T0
    T0IF = 0;

    _u8ClockITR++;

    if (!(_u8ClockITR % WRAPS_A_SEC))
    {
		volatile unsigned long int test = SECS_A_DAY;

      if (_u8ClockITR >= (WRAPS_A_SEC * ITR_RESET_AFTER))
      {
        //DCF needs to use this in order to find sync
        //therefore _u8ClockITR needs to run longer than 2 seconds (in this case ITR_RESET_AFTER)
        _u8ClockITR = 0;
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
      static unsigned char _u8LastTime    = 0,
                           _u8CurrentBit,
                           _u8CurrentByte;
      unsigned short int   u16TimeSince;

      #if DEBUG
        COL0 = !COL0;
      #endif

      _bRb2 = RB2;

      if (_u8ClockITR >= _u8LastTime)
      {
        u16TimeSince = _u8ClockITR - _u8LastTime;
      }
      else
      {
        u16TimeSince = _u8ClockITR + (WRAPS_A_SEC * ITR_RESET_AFTER) - _u8LastTime;
      }

      _u8LastTime = _u8ClockITR;

      if (!_u8DCFBitsRecvd)
      {
        _u8CurrentBit  = 7;
        _u8CurrentByte = 0;
        _au8DCFData[_u8CurrentByte] = 0;
      }

      if (_bRb2 == 0)
      {
        //falling edge since we are using an inverted output
        if ((u16TimeSince > (WRAPS_A_SEC * 3 / 50)) &&
            (u16TimeSince < (WRAPS_A_SEC * 7 / 50)))
        {
          //about 100ms is a '0'

          #if DEBUG
            COL1 = !COL1;
          #endif

          if (_u8DCFBitsRecvd < (2 * 59))
          {
            //we do not need to clear the bit here since it is pre-cleared

            _u8DCFBitsRecvd++;

            if (_u8CurrentBit)
            {
              _u8CurrentBit--;
            }
            else
            {
              _u8CurrentBit = 7;
              _u8CurrentByte++;
              _au8DCFData[_u8CurrentByte] = 0;
            }
          }
        }
        else if ((u16TimeSince > (WRAPS_A_SEC *  8 / 50)) &&
                 (u16TimeSince < (WRAPS_A_SEC * 12 / 50)))
        {
          //about 200ms is a '1'

          #if DEBUG
            COL2 = !COL2;
          #endif

          if (_u8DCFBitsRecvd < (2 * 59))
          {
            BIT_SET(_au8DCFData[_u8CurrentByte], _u8CurrentBit);

            _u8DCFBitsRecvd++;

            if (_u8CurrentBit)
            {
              _u8CurrentBit--;
            }
            else
            {
              _u8CurrentBit = 7;
              _u8CurrentByte++;
              _au8DCFData[_u8CurrentByte] = 0;
            }
          }
        }
        else
        {
          //invalid interval length

          #if DEBUG
            COL10 = !COL10;
          #endif

          _u8DCFSyncDone  = 0;
          _u8DCFBitsRecvd = 0;
        }

        if (_u8DCFBitsRecvd > 59 &&
            !_u8DCFSyncDone)
        {
          //something went wrong since there would have had to be a sync before byte 60

          #if DEBUG
            COL9 = !COL9;
          #endif

          _u8DCFSyncDone  = 0;
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

          #if DEBUG
            COL3 = !COL3;
          #endif

          if (_u8DCFBitsRecvd == 59)
          {
            //we have a complete data set available
            //if we do not yet have a time set use the time we have found

            if (!_u8ValidTime)
            {
              unsigned char u8Hour   = 0;
              unsigned char u8Minute = 0;

              //bit 21
              if (_au8DCFData[2] & 0x4)
              {
                u8Minute += 1;
              }
              //bit 22
              if (_au8DCFData[2] & 0x2)
              {
                u8Minute += 2;
              }
              //bit 23
              if (_au8DCFData[2] & 0x1)
              {
                u8Minute += 4;
              }
              //bit 24
              if (_au8DCFData[3] & 0x80)
              {
                u8Minute += 8;
              }
              //bit 25
              if (_au8DCFData[3] & 0x40)
              {
                u8Minute += 10;
              }
              //bit 26
              if (_au8DCFData[3] & 0x20)
              {
                u8Minute += 20;
              }
              //bit 27
              if (_au8DCFData[3] & 0x10)
              {
                u8Minute += 40;
              }

              //bit 29
              if (_au8DCFData[3] & 0x4)
              {
                u8Hour += 1;
              }
              //bit 30
              if (_au8DCFData[3] & 0x2)
              {
                u8Hour += 2;
              }
              //bit 31
              if (_au8DCFData[3] & 0x1)
              {
                u8Hour += 4;
              }
              //bit 32
              if (_au8DCFData[4] & 0x80)
              {
                u8Hour += 8;
              }
              //bit 33
              if (_au8DCFData[4] & 0x40)
              {
                u8Hour += 10;
              }
              //bit 34
              if (_au8DCFData[4] & 0x20)
              {
                u8Hour += 20;
              }

              _u24SecondsToday = u8Hour * 60 * 60/* +
                                 u8Minute * 60*/;

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
            #if DEBUG
              COL8 = !COL8;
            #endif
          
            _u8DCFBitsRecvd = 0;
          }
        }
        else if (u16TimeSince >= (5 * WRAPS_A_SEC / 2))
        {
          //should not happen

          #if DEBUG
            COL7 = !COL7;
          #endif

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

  //timer prescaler set to 1/256
  PS2 = 1;
  PS1 = 1;
  PS0 = 1;

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

    #if DEBUG
      //use row 0 for debug output
      ROW0 = 1;
    #else
      if (_u8ValidTime)
      {
        vWriteTime(_u24SecondsToday, 1);
      }
      else
      {
        unsigned char au8Pattern[((NUM_COLS * NUM_ROWS) / 8) + 1];

        memset(au8Pattern, 0, sizeof(au8Pattern));
        if (_u24SecondsToday & 0x1)
        {
          BIT_SET(au8Pattern[0], 0);
        }

        vWritePattern(au8Pattern);
      }
    #endif
  }
}
