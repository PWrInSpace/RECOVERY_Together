// Copyright 2022 PWrInSpace, Kuba
#ifndef CONSOLE_CONFIG_H
#define CONSOLE_CONFIG_H

#include "console.h"
#include "esp_log.h"
#include "esp_err.h"
#include "servo_config.h"
#include "servo_control.h"
#include "recovery_config.h"
#include "cots.h"
#include "cots_config.h"

/**
 * @brief Initialize cli
 *
 * @return esp_err_t initialization status
 */
esp_err_t init_console(void);

#endif