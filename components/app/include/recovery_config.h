#ifndef RECOVERY_CONFIG_H
#define RECOVERY_CONFIG_H

#include "recovery.h"
#include "servo_control.h"
#include "servo_config.h"

extern recovery_t recovery;

esp_err_t init_recovery(void);

esp_err_t resistance_wire_on_time(int time);

#endif
