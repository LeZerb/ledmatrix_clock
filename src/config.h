#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"

    typedef enum {
        eCONF_ES_IST           = 0x01, //bit 0 identifies es ist mode
        eCONF_VIERTEL_VOR_NACH = 0x02, //bit 1 identifies viertel nach/vor mode
        eCONF_ZWANZIG_VOR_NACH = 0x04, //bit 2 identifies zwanzig nach/vor mode
        eCONF_MITTERNACHT      = 0x08, //bit 3 identifies mitternacht enable
        eCONF_NULL             = 0x10, //bit 4 identifies null uhr mode
        eCONF_MAX_BRIGHTNESS   = 7,    //3 bits for 8 brightness values
        eCONF_BRIGHTNESS_SHIFT = 5,    //shift value for brightness setting
        eCONF_VALID            = 0xA5  //byte signifying valid config
    } TE_CONFIG;

    void configSave(U8 eConfig);
    inline U8 configGet();

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */
