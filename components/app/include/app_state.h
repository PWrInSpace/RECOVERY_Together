#ifndef APP_STATE_H
#define APP_STATE_H

#include "cots.h"
#include "recovery.h"
#include "logger.h"
#include "cots_config.h"
#include "recovery_config.h"
#include "logger_config.h"
#include "esp_err.h"
#include "i2c.h"
#include "commands_config.h"
#include "adc.h"

typedef struct {
    cots_data_t telemetrum_data;
    cots_data_t easymini_data;
    recovery_data_t recovery_data;
    bool continuity: 1;
} app_state_t;

typedef struct {
    bool telemetrum_armed: 1;
    bool telemetrum_apogee_detected: 1;
    bool telemetrum_first_stage: 1;
    bool telemetrum_second_stage: 1;
    bool easymini_armed: 1;
    bool easymini_apogee_detected: 1;
    bool easymini_first_stage: 1;
    bool easymini_second_stage: 1;
    bool separation_one: 1;
    bool separation_two: 1;
    bool continuity: 1;
} i2c_data_t;

extern app_state_t app_state;

esp_err_t init_app_state(void);

esp_err_t init_app_state_test(void);

#endif
