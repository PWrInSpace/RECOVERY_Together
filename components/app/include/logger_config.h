#ifndef LOGGER_CONFIG_H
#define LOGGER_CONFIG_H

#include "logger.h"
#include "app_state.h"

extern logger_task_t logger_task;

esp_err_t init_logger_task(void);

#endif
