#include "common.h"
#include "eeprom.h"
#include "config.h"

#define DEFAULT_CONFIG (eCONF_VALID | eCONF_ES_IST)

static BOOL _wasLoaded = FALSE;
static U8 _config = 0;

void configLoad() {
    _config = eeprom_read(eEEP_CONFIG_OFFSET);

    if ((_config & eCONF_INVALID_MASK) ||
            ((_config & eCONF_VALID)) != eCONF_VALID) {
        _config = DEFAULT_CONFIG;
    }

    _wasLoaded = TRUE;
}

void configSave(TE_CONFIG eConfig) {
    U8 config;

    config = eConfig;
    config &= ~eCONF_INVALID_MASK;
    config |= eCONF_VALID;

    if (!_wasLoaded) {
        configLoad();
    }

    if (_config == config) {
        return;
    }

    eeprom_write(eEEP_CONFIG_OFFSET, config);
    //read again in order to wait for the eeprom write to complete
    _config = eeprom_read(eEEP_CONFIG_OFFSET);
}

inline TE_CONFIG configGet() {
    if (!_wasLoaded) {
        configLoad();
    }

    return _config;
}