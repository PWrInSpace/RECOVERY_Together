#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"
#include "esp_log.h"

typedef void (*command_callback)(void);

typedef struct {
    uint8_t command_id;
    command_callback command_fnc;
} command_t;

esp_err_t process_command(uint8_t command_id, const command_t *commands, size_t commands_count);

#endif
