#include "common.h"

//which PINS are to be used as output
//columns
#define OUTPUT0  TRISA0
#define OUTPUT1  TRISA1
#define OUTPUT2  TRISA2
#define OUTPUT3  TRISA3
#define OUTPUT4  TRISA4
#define OUTPUT5  TRISA5
#define OUTPUT6  TRISE0
#define OUTPUT7  TRISE1
#define OUTPUT8  TRISE2
#define OUTPUT9  TRISC0
#define OUTPUT10 TRISC1
//rows
#define OUTPUT11 TRISB1
#define OUTPUT12 TRISB0
#define OUTPUT13 TRISD7
#define OUTPUT14 TRISD6
#define OUTPUT15 TRISD5
#define OUTPUT16 TRISD4
#define OUTPUT17 TRISC7
#define OUTPUT18 TRISC6
#define OUTPUT19 TRISC5
#define OUTPUT20 TRISC4
#define OUTPUT21 TRISD3
#define OUTPUT23 TRISD2
//DCF power
#define OUTPUT24 TRISC3

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
#pragma config DEBUG=OFF
#pragma config LVP=OFF
#pragma config FCMEN=ON
#pragma config IESO=OFF
#pragma config BOREN=ON
#pragma config CPD=OFF
#pragma config CP=OFF
#pragma config MCLRE=OFF
#pragma config PWRTE=ON
#pragma config WDTE=OFF
#pragma config FOSC=HS

/*
 ** flash write protection is not enabled
 ** brown-out reset on 2.1V
 */
#pragma config WRT=OFF
#pragma config BOR4V=BOR21V

void vInit() {
    //enable watchdog timer with prescaler 1:65536
    WDTPS3 = 1;
    WDTPS2 = 0;
    WDTPS1 = 1;
    WDTPS0 = 1;
    SWDTEN = 0;

    //disable port B pull-ups
    nRBPU = 1;
    WPUB = 0;

    //timer 0 clock source is internal clock Fosc/4
    T0CS = 0;

    //prescaler is assigned to timer 0
    PSA = 0;

    //timer prescaler set to 1/256
    PS2 = 1;
    PS1 = 1;
    PS0 = 1;

    //8MHz - which is not used since we are using an external crystal
    IRCF2 = 1;
    IRCF1 = 1;
    IRCF1 = 1;

    //globally enable interrupts
    GIE = 1;

    //enable timer 0 wrap around interrupt
    T0IE = 1;

    //disable analog inputs
    ANSEL = 0;
    ANSELH = 0;

    //enable A/D on RB1/AN10
    //ANS10 = 1;

    //enable peripheral + A/D interrupt
    //ADIF = 0;
    //ADIE = 1;
    //PEIE = 1;

    //configure A/D
    //use FRC
    //ADCS1 = 1;
    //ADCS0 = 1;
    //use RB1/AN10
    //CHS3  = 1;
    //CHS2  = 0;
    //CHS1  = 1;
    //CHS0  = 0;
    //right justified 2 bits in ADRESH 8 bits in ADRESL
    //ADFM  = 1;
    //voltage references are Vss and Vdd
    //VCFG1 = 0;
    //VCFG0 = 0;

    //initialize output latches to 0
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    //configure output ports as output
    OUTPUT0 = 0;
    OUTPUT1 = 0;
    OUTPUT2 = 0;
    OUTPUT3 = 0;
    OUTPUT4 = 0;
    OUTPUT5 = 0;
    OUTPUT6 = 0;
    OUTPUT7 = 0;
    OUTPUT8 = 0;
    OUTPUT9 = 0;
    OUTPUT10 = 0;
    OUTPUT11 = 0;
    OUTPUT12 = 0;
    OUTPUT13 = 0;
    OUTPUT14 = 0;
    OUTPUT15 = 0;
    OUTPUT16 = 0;
    OUTPUT17 = 0;
    OUTPUT18 = 0;
    OUTPUT19 = 0;
    OUTPUT20 = 0;
    OUTPUT21 = 0;
    OUTPUT23 = 0;
    OUTPUT24 = 0;

    //enable interrupt on port RB2 (DCF), RB3 and RB4 (switches)
    IOCB2 = 1;
    IOCB3 = 1;
    IOCB4 = 1;

    //enable port B interrupt
    RBIE = 1;
}
