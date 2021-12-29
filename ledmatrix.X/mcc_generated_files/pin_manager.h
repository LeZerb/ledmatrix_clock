/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC18F45K22
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set COL00 aliases
#define COL00_TRIS               TRISAbits.TRISA0
#define COL00_LAT                LATAbits.LATA0
#define COL00_PORT               PORTAbits.RA0
#define COL00_ANS                ANSELAbits.ANSA0
#define COL00_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define COL00_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define COL00_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define COL00_GetValue()           PORTAbits.RA0
#define COL00_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define COL00_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define COL00_SetAnalogMode()  do { ANSELAbits.ANSA0 = 1; } while(0)
#define COL00_SetDigitalMode() do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set COL01 aliases
#define COL01_TRIS               TRISAbits.TRISA1
#define COL01_LAT                LATAbits.LATA1
#define COL01_PORT               PORTAbits.RA1
#define COL01_ANS                ANSELAbits.ANSA1
#define COL01_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define COL01_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define COL01_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define COL01_GetValue()           PORTAbits.RA1
#define COL01_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define COL01_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define COL01_SetAnalogMode()  do { ANSELAbits.ANSA1 = 1; } while(0)
#define COL01_SetDigitalMode() do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set COL02 aliases
#define COL02_TRIS               TRISAbits.TRISA2
#define COL02_LAT                LATAbits.LATA2
#define COL02_PORT               PORTAbits.RA2
#define COL02_ANS                ANSELAbits.ANSA2
#define COL02_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define COL02_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define COL02_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define COL02_GetValue()           PORTAbits.RA2
#define COL02_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define COL02_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define COL02_SetAnalogMode()  do { ANSELAbits.ANSA2 = 1; } while(0)
#define COL02_SetDigitalMode() do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set COL03 aliases
#define COL03_TRIS               TRISAbits.TRISA3
#define COL03_LAT                LATAbits.LATA3
#define COL03_PORT               PORTAbits.RA3
#define COL03_ANS                ANSELAbits.ANSA3
#define COL03_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define COL03_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define COL03_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define COL03_GetValue()           PORTAbits.RA3
#define COL03_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define COL03_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define COL03_SetAnalogMode()  do { ANSELAbits.ANSA3 = 1; } while(0)
#define COL03_SetDigitalMode() do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set COL04 aliases
#define COL04_TRIS               TRISAbits.TRISA4
#define COL04_LAT                LATAbits.LATA4
#define COL04_PORT               PORTAbits.RA4
#define COL04_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define COL04_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define COL04_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define COL04_GetValue()           PORTAbits.RA4
#define COL04_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define COL04_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)

// get/set COL05 aliases
#define COL05_TRIS               TRISAbits.TRISA5
#define COL05_LAT                LATAbits.LATA5
#define COL05_PORT               PORTAbits.RA5
#define COL05_ANS                ANSELAbits.ANSA5
#define COL05_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define COL05_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define COL05_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define COL05_GetValue()           PORTAbits.RA5
#define COL05_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define COL05_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define COL05_SetAnalogMode()  do { ANSELAbits.ANSA5 = 1; } while(0)
#define COL05_SetDigitalMode() do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set ROW01 aliases
#define ROW01_TRIS               TRISBbits.TRISB0
#define ROW01_LAT                LATBbits.LATB0
#define ROW01_PORT               PORTBbits.RB0
#define ROW01_WPU                WPUBbits.WPUB0
#define ROW01_ANS                ANSELBbits.ANSB0
#define ROW01_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define ROW01_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define ROW01_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define ROW01_GetValue()           PORTBbits.RB0
#define ROW01_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define ROW01_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define ROW01_SetPullup()      do { WPUBbits.WPUB0 = 1; } while(0)
#define ROW01_ResetPullup()    do { WPUBbits.WPUB0 = 0; } while(0)
#define ROW01_SetAnalogMode()  do { ANSELBbits.ANSB0 = 1; } while(0)
#define ROW01_SetDigitalMode() do { ANSELBbits.ANSB0 = 0; } while(0)

// get/set ROW00 aliases
#define ROW00_TRIS               TRISBbits.TRISB1
#define ROW00_LAT                LATBbits.LATB1
#define ROW00_PORT               PORTBbits.RB1
#define ROW00_WPU                WPUBbits.WPUB1
#define ROW00_ANS                ANSELBbits.ANSB1
#define ROW00_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define ROW00_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define ROW00_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define ROW00_GetValue()           PORTBbits.RB1
#define ROW00_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define ROW00_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define ROW00_SetPullup()      do { WPUBbits.WPUB1 = 1; } while(0)
#define ROW00_ResetPullup()    do { WPUBbits.WPUB1 = 0; } while(0)
#define ROW00_SetAnalogMode()  do { ANSELBbits.ANSB1 = 1; } while(0)
#define ROW00_SetDigitalMode() do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set DCFIN aliases
#define DCFIN_TRIS               TRISBbits.TRISB2
#define DCFIN_LAT                LATBbits.LATB2
#define DCFIN_PORT               PORTBbits.RB2
#define DCFIN_WPU                WPUBbits.WPUB2
#define DCFIN_ANS                ANSELBbits.ANSB2
#define DCFIN_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define DCFIN_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define DCFIN_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define DCFIN_GetValue()           PORTBbits.RB2
#define DCFIN_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define DCFIN_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define DCFIN_SetPullup()      do { WPUBbits.WPUB2 = 1; } while(0)
#define DCFIN_ResetPullup()    do { WPUBbits.WPUB2 = 0; } while(0)
#define DCFIN_SetAnalogMode()  do { ANSELBbits.ANSB2 = 1; } while(0)
#define DCFIN_SetDigitalMode() do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set BTNSET aliases
#define BTNSET_TRIS               TRISBbits.TRISB3
#define BTNSET_LAT                LATBbits.LATB3
#define BTNSET_PORT               PORTBbits.RB3
#define BTNSET_WPU                WPUBbits.WPUB3
#define BTNSET_ANS                ANSELBbits.ANSB3
#define BTNSET_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define BTNSET_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define BTNSET_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define BTNSET_GetValue()           PORTBbits.RB3
#define BTNSET_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define BTNSET_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define BTNSET_SetPullup()      do { WPUBbits.WPUB3 = 1; } while(0)
#define BTNSET_ResetPullup()    do { WPUBbits.WPUB3 = 0; } while(0)
#define BTNSET_SetAnalogMode()  do { ANSELBbits.ANSB3 = 1; } while(0)
#define BTNSET_SetDigitalMode() do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set BTNMENU aliases
#define BTNMENU_TRIS               TRISBbits.TRISB4
#define BTNMENU_LAT                LATBbits.LATB4
#define BTNMENU_PORT               PORTBbits.RB4
#define BTNMENU_WPU                WPUBbits.WPUB4
#define BTNMENU_ANS                ANSELBbits.ANSB4
#define BTNMENU_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define BTNMENU_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define BTNMENU_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define BTNMENU_GetValue()           PORTBbits.RB4
#define BTNMENU_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define BTNMENU_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define BTNMENU_SetPullup()      do { WPUBbits.WPUB4 = 1; } while(0)
#define BTNMENU_ResetPullup()    do { WPUBbits.WPUB4 = 0; } while(0)
#define BTNMENU_SetAnalogMode()  do { ANSELBbits.ANSB4 = 1; } while(0)
#define BTNMENU_SetDigitalMode() do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set COL09 aliases
#define COL09_TRIS               TRISCbits.TRISC0
#define COL09_LAT                LATCbits.LATC0
#define COL09_PORT               PORTCbits.RC0
#define COL09_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define COL09_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define COL09_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define COL09_GetValue()           PORTCbits.RC0
#define COL09_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define COL09_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)

// get/set COL10 aliases
#define COL10_TRIS               TRISCbits.TRISC1
#define COL10_LAT                LATCbits.LATC1
#define COL10_PORT               PORTCbits.RC1
#define COL10_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define COL10_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define COL10_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define COL10_GetValue()           PORTCbits.RC1
#define COL10_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define COL10_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)

// get/set DCFPWR aliases
#define DCFPWR_TRIS               TRISCbits.TRISC3
#define DCFPWR_LAT                LATCbits.LATC3
#define DCFPWR_PORT               PORTCbits.RC3
#define DCFPWR_ANS                ANSELCbits.ANSC3
#define DCFPWR_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define DCFPWR_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define DCFPWR_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define DCFPWR_GetValue()           PORTCbits.RC3
#define DCFPWR_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define DCFPWR_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define DCFPWR_SetAnalogMode()  do { ANSELCbits.ANSC3 = 1; } while(0)
#define DCFPWR_SetDigitalMode() do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set ROW09 aliases
#define ROW09_TRIS               TRISCbits.TRISC4
#define ROW09_LAT                LATCbits.LATC4
#define ROW09_PORT               PORTCbits.RC4
#define ROW09_ANS                ANSELCbits.ANSC4
#define ROW09_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define ROW09_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define ROW09_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define ROW09_GetValue()           PORTCbits.RC4
#define ROW09_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define ROW09_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define ROW09_SetAnalogMode()  do { ANSELCbits.ANSC4 = 1; } while(0)
#define ROW09_SetDigitalMode() do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set ROW08 aliases
#define ROW08_TRIS               TRISCbits.TRISC5
#define ROW08_LAT                LATCbits.LATC5
#define ROW08_PORT               PORTCbits.RC5
#define ROW08_ANS                ANSELCbits.ANSC5
#define ROW08_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define ROW08_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define ROW08_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define ROW08_GetValue()           PORTCbits.RC5
#define ROW08_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define ROW08_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define ROW08_SetAnalogMode()  do { ANSELCbits.ANSC5 = 1; } while(0)
#define ROW08_SetDigitalMode() do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set ROW07 aliases
#define ROW07_TRIS               TRISCbits.TRISC6
#define ROW07_LAT                LATCbits.LATC6
#define ROW07_PORT               PORTCbits.RC6
#define ROW07_ANS                ANSELCbits.ANSC6
#define ROW07_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define ROW07_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define ROW07_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define ROW07_GetValue()           PORTCbits.RC6
#define ROW07_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define ROW07_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define ROW07_SetAnalogMode()  do { ANSELCbits.ANSC6 = 1; } while(0)
#define ROW07_SetDigitalMode() do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set ROW06 aliases
#define ROW06_TRIS               TRISCbits.TRISC7
#define ROW06_LAT                LATCbits.LATC7
#define ROW06_PORT               PORTCbits.RC7
#define ROW06_ANS                ANSELCbits.ANSC7
#define ROW06_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define ROW06_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define ROW06_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define ROW06_GetValue()           PORTCbits.RC7
#define ROW06_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define ROW06_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define ROW06_SetAnalogMode()  do { ANSELCbits.ANSC7 = 1; } while(0)
#define ROW06_SetDigitalMode() do { ANSELCbits.ANSC7 = 0; } while(0)

// get/set ROW11 aliases
#define ROW11_TRIS               TRISDbits.TRISD2
#define ROW11_LAT                LATDbits.LATD2
#define ROW11_PORT               PORTDbits.RD2
#define ROW11_ANS                ANSELDbits.ANSD2
#define ROW11_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define ROW11_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define ROW11_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define ROW11_GetValue()           PORTDbits.RD2
#define ROW11_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define ROW11_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define ROW11_SetAnalogMode()  do { ANSELDbits.ANSD2 = 1; } while(0)
#define ROW11_SetDigitalMode() do { ANSELDbits.ANSD2 = 0; } while(0)

// get/set ROW10 aliases
#define ROW10_TRIS               TRISDbits.TRISD3
#define ROW10_LAT                LATDbits.LATD3
#define ROW10_PORT               PORTDbits.RD3
#define ROW10_ANS                ANSELDbits.ANSD3
#define ROW10_SetHigh()            do { LATDbits.LATD3 = 1; } while(0)
#define ROW10_SetLow()             do { LATDbits.LATD3 = 0; } while(0)
#define ROW10_Toggle()             do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define ROW10_GetValue()           PORTDbits.RD3
#define ROW10_SetDigitalInput()    do { TRISDbits.TRISD3 = 1; } while(0)
#define ROW10_SetDigitalOutput()   do { TRISDbits.TRISD3 = 0; } while(0)
#define ROW10_SetAnalogMode()  do { ANSELDbits.ANSD3 = 1; } while(0)
#define ROW10_SetDigitalMode() do { ANSELDbits.ANSD3 = 0; } while(0)

// get/set ROW05 aliases
#define ROW05_TRIS               TRISDbits.TRISD4
#define ROW05_LAT                LATDbits.LATD4
#define ROW05_PORT               PORTDbits.RD4
#define ROW05_ANS                ANSELDbits.ANSD4
#define ROW05_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define ROW05_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define ROW05_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define ROW05_GetValue()           PORTDbits.RD4
#define ROW05_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define ROW05_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define ROW05_SetAnalogMode()  do { ANSELDbits.ANSD4 = 1; } while(0)
#define ROW05_SetDigitalMode() do { ANSELDbits.ANSD4 = 0; } while(0)

// get/set ROW04 aliases
#define ROW04_TRIS               TRISDbits.TRISD5
#define ROW04_LAT                LATDbits.LATD5
#define ROW04_PORT               PORTDbits.RD5
#define ROW04_ANS                ANSELDbits.ANSD5
#define ROW04_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define ROW04_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define ROW04_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define ROW04_GetValue()           PORTDbits.RD5
#define ROW04_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define ROW04_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define ROW04_SetAnalogMode()  do { ANSELDbits.ANSD5 = 1; } while(0)
#define ROW04_SetDigitalMode() do { ANSELDbits.ANSD5 = 0; } while(0)

// get/set ROW03 aliases
#define ROW03_TRIS               TRISDbits.TRISD6
#define ROW03_LAT                LATDbits.LATD6
#define ROW03_PORT               PORTDbits.RD6
#define ROW03_ANS                ANSELDbits.ANSD6
#define ROW03_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define ROW03_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define ROW03_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define ROW03_GetValue()           PORTDbits.RD6
#define ROW03_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define ROW03_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define ROW03_SetAnalogMode()  do { ANSELDbits.ANSD6 = 1; } while(0)
#define ROW03_SetDigitalMode() do { ANSELDbits.ANSD6 = 0; } while(0)

// get/set ROW02 aliases
#define ROW02_TRIS               TRISDbits.TRISD7
#define ROW02_LAT                LATDbits.LATD7
#define ROW02_PORT               PORTDbits.RD7
#define ROW02_ANS                ANSELDbits.ANSD7
#define ROW02_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define ROW02_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define ROW02_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define ROW02_GetValue()           PORTDbits.RD7
#define ROW02_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define ROW02_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define ROW02_SetAnalogMode()  do { ANSELDbits.ANSD7 = 1; } while(0)
#define ROW02_SetDigitalMode() do { ANSELDbits.ANSD7 = 0; } while(0)

// get/set COL06 aliases
#define COL06_TRIS               TRISEbits.TRISE0
#define COL06_LAT                LATEbits.LATE0
#define COL06_PORT               PORTEbits.RE0
#define COL06_ANS                ANSELEbits.ANSE0
#define COL06_SetHigh()            do { LATEbits.LATE0 = 1; } while(0)
#define COL06_SetLow()             do { LATEbits.LATE0 = 0; } while(0)
#define COL06_Toggle()             do { LATEbits.LATE0 = ~LATEbits.LATE0; } while(0)
#define COL06_GetValue()           PORTEbits.RE0
#define COL06_SetDigitalInput()    do { TRISEbits.TRISE0 = 1; } while(0)
#define COL06_SetDigitalOutput()   do { TRISEbits.TRISE0 = 0; } while(0)
#define COL06_SetAnalogMode()  do { ANSELEbits.ANSE0 = 1; } while(0)
#define COL06_SetDigitalMode() do { ANSELEbits.ANSE0 = 0; } while(0)

// get/set COL07 aliases
#define COL07_TRIS               TRISEbits.TRISE1
#define COL07_LAT                LATEbits.LATE1
#define COL07_PORT               PORTEbits.RE1
#define COL07_ANS                ANSELEbits.ANSE1
#define COL07_SetHigh()            do { LATEbits.LATE1 = 1; } while(0)
#define COL07_SetLow()             do { LATEbits.LATE1 = 0; } while(0)
#define COL07_Toggle()             do { LATEbits.LATE1 = ~LATEbits.LATE1; } while(0)
#define COL07_GetValue()           PORTEbits.RE1
#define COL07_SetDigitalInput()    do { TRISEbits.TRISE1 = 1; } while(0)
#define COL07_SetDigitalOutput()   do { TRISEbits.TRISE1 = 0; } while(0)
#define COL07_SetAnalogMode()  do { ANSELEbits.ANSE1 = 1; } while(0)
#define COL07_SetDigitalMode() do { ANSELEbits.ANSE1 = 0; } while(0)

// get/set COL08 aliases
#define COL08_TRIS               TRISEbits.TRISE2
#define COL08_LAT                LATEbits.LATE2
#define COL08_PORT               PORTEbits.RE2
#define COL08_ANS                ANSELEbits.ANSE2
#define COL08_SetHigh()            do { LATEbits.LATE2 = 1; } while(0)
#define COL08_SetLow()             do { LATEbits.LATE2 = 0; } while(0)
#define COL08_Toggle()             do { LATEbits.LATE2 = ~LATEbits.LATE2; } while(0)
#define COL08_GetValue()           PORTEbits.RE2
#define COL08_SetDigitalInput()    do { TRISEbits.TRISE2 = 1; } while(0)
#define COL08_SetDigitalOutput()   do { TRISEbits.TRISE2 = 0; } while(0)
#define COL08_SetAnalogMode()  do { ANSELEbits.ANSE2 = 1; } while(0)
#define COL08_SetDigitalMode() do { ANSELEbits.ANSE2 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/