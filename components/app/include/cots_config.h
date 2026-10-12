#ifndef COTS_CONFIG_H
#define COTS_CONFIG_H

#include "cots.h"
#include "recovery_config.h"

extern cots_t telemetrum;
extern cots_t bluejay;

esp_err_t init_telemetrum(void);

esp_err_t init_bluejay(void);

#endif
