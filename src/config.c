#include "common.h"
#include "eeprom.h"
#include "config.h"

#define DEFAULT_CONFIG (eCONF_BRIGHTNESS | eCONF_ES_IST)

static BOOL _wasLoaded = FALSE;
static U8 _config = 0;

void configLoad() {
    U8 valid = eeprom_read(eEEP_CONFIG_VALID_OFFSET);
    _config = eeprom_read(eEEP_CONFIG_OFFSET);

    if (valid != eCONF_VALID) {
        _config = DEFAULT_CONFIG;
    }

    _wasLoaded = TRUE;
}

void configSave(TE_CONFIG eConfig) {
    U8 config;

    config = eConfig;

    if (!_wasLoaded) {
        configLoad();
    }

    if (_config == config) {
        return;
    }

    eeprom_write(eEEP_CONFIG_OFFSET, config);
    eeprom_write(eEEP_CONFIG_VALID_OFFSET, eCONF_VALID);
    //read again in order to wait for the eeprom writes to complete
    _config = eeprom_read(eEEP_CONFIG_OFFSET);
}

inline TE_CONFIG configGet() {
    if (!_wasLoaded) {
        configLoad();
    }

    return _config;
}