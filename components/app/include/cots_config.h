#ifndef COTS_CONFIG_H
#define COTS_CONFIG_H

#include "cots.h"

typedef enum {
    TELEMETRUM = 0,
    EASYMINI
} cots_e;

extern cots_t telemetrum;
extern cots_t easymini;

esp_err_t init_telemetrum(void);

esp_err_t init_easymini(void);

#endif
