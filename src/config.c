#include "common.h"
#include "eeprom.h"
#include "config.h"

#define DEFAULT_CONFIG (eCONF_BRIGHTNESS | eCONF_ES_IST)

static BOOL _wasLoaded = FALSE;
static U8 _config = DEFAULT_CONFIG;

void configLoad() {
    U8 valid = eeprom_read(eEEP_CONFIG_VALID_OFFSET);

    if (valid == eCONF_VALID) {
        _config = eeprom_read(eEEP_CONFIG_OFFSET);
    }

    _wasLoaded = TRUE;
}

void configSave(U8 config) {
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

inline U8 configGet() {
    if (!_wasLoaded) {
        configLoad();
    }

    return _config;
}
