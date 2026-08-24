// Copyright 2022 PWrInSpace, Kuba

#ifndef CONSOLE_H
#define CONSOLE_H

#include "esp_console.h"
#include "esp_err.h"
#include "esp_log.h"

// For some serial monitors this lib is crashing
// Works with cutecom ...
// Print giga good message to default stream
#define CONSOLE_WRITE_G(format, ...) ESP_LOGI("CLI", format, ##__VA_ARGS__)

// Print error message to default stream
#define CONSOLE_WRITE_E(format, ...) ESP_LOGE("CLI", format, ##__VA_ARGS__)

// Print to default stream
#define CONSOLE_WRITE(format, ...) ESP_LOGI("CLI", format, ##__VA_ARGS__)

esp_err_t console_init(void);

esp_err_t console_register_commands(esp_console_cmd_t *commands, size_t number_of_cmd);

esp_err_t console_deinit(void);

#endif
