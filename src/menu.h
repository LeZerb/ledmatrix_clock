#ifndef _MENU_H_
#define _MENU_H_

#include "config.h"
#include "date.h"

//typedefs

typedef enum {
    //do not change menu item order
    eMENU_NIRVANA, //initial state
    eMENU_MAIN_FIRST, //first main menu entry
    eMENU_MAIN_CONFIG = eMENU_MAIN_FIRST, //main menu config
    eMENU_MAIN_TIME, //main menu time
    eMENU_MAIN_INVALIDATE_TIME, //main menu invalidate time
    eMENU_MAIN_TEST_DISPLAY, //main menu test display
    eMENU_MAIN_LAST = eMENU_MAIN_TEST_DISPLAY, //last main menu entrya
    eMENU_SET_CONFIG_ES_IST, //change config for es ist
    eMENU_SET_CONFIG_VIERTEL_VOR_NACH, //change config for viertel vor/nach mode
    eMENU_SET_CONFIG_ZWANZIG_VOR_NACH, //change config for zwanzig vor/nach mode
    eMENU_SET_CONFIG_MITTERNACHT, //change config for mitternacht mode
    eMENU_SET_CONFIG_NULL, //change config for null uhr mode
    eMENU_SET_HOUR, //set hour menu
    eMENU_SET_MINUTE, //set minute menu
    eMENU_SET_DAY, //set day menu
    eMENU_SET_MONTH, //set month menu
    eMENU_SET_YEAR, //set year menu
    eMENU_SHOW_TIME, //show time
    eMENU_SHOW_SECOND, //show second
    eMENU_SHOW_DAY, //show day
    eMENU_SHOW_YEAR, //show year
    eMENU_ENTRY_COUNT
} TE_MENU_STATE;

typedef enum {
    //do not change the button order
    eBUTTON_MENU,
    eBUTTON_SET,
    eBUTTON_COUNT
} TE_BUTTONS;

//functions

//return TRUE when we are in the menu
void eHandleButton(TE_BUTTONS eButton);

//return menu state
inline TE_MENU_STATE menuGetState();

#endif
