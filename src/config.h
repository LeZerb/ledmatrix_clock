#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"

    typedef enum {
        eCONF_ES_IST = 0x01, //bit 0 identifies es ist mode
        eCONF_VIERTEL_VOR_NACH = 0x02, //bit 1 identifies viertel nach/vor mode
        eCONF_ZWANZIG_VOR_NACH = 0x04, //bit 2 identifies zwanzig nach/vor mode
        eCONF_MITTERNACHT = 0x08, //bit 3 identifies mitternacht enable
        eCONF_NULL = 0x10, //bit 4 identifies null uhr mode
        eCONF_VALID = 0x80, //msb identifies valid config when read from eeprom
        eCONF_INVALID_MASK = 0x70
    } TE_CONFIG;

    void configSave(TE_CONFIG eConfig);
    inline TE_CONFIG configGet();

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */
