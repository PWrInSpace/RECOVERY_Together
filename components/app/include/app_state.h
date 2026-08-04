#ifndef APP_STATE_H
#define APP_STATE_H

#include "cots.h"
#include "recovery.h"
#include "logger.h"
#include "cots_config.h"
#include "recovery_config.h"
#include "logger_config.h"
#include "esp_err.h"

typedef struct {
    cots_data_t telemetrum_data;
    cots_data_t easymini_data;
    recovery_data_t recovery_data;
} app_state_t;

extern app_state_t app_state;

esp_err_t init_app_state(void);

#endif
