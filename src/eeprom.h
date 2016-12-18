/*
 * File:   eeprom.h
 * Author: Matthias
 *
 * Created on 1. November 2016, 09:03
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

    enum EEPROM_OFFSETS {
        eEEP_CONFIG_OFFSET = 0,
        eEEP_CONFIG_VALID_OFFSET = 1
    };


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

