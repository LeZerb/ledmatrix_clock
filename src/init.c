#include "common.h"

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

void vInit()
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
