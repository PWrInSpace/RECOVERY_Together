#ifndef APP_H
#define APP_H

#include "cots_config.h"
#include "servo_config.h"
#include "recovery_config.h"
#include "console_config.h"
#include "logger_config.h"
#include "spi_config.h"
#include "app_state.h"

esp_err_t app_init(void);

#endif