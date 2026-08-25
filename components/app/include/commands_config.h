#ifndef COMMANDS_CONFIG_H
#define COMMANDS_CONFIG_H

#include "commands.h"
#include "cots_config.h"
#include "cots.h"
#include "i2c.h"
#include "recovery_config.h"

typedef enum {
    EASYMINI_ARM_CMD = 0x01,
    EASYMINI_DISARM_CMD = 0x02,
    TELEMETRUM_ARM_CMD = 0x03,
    TELEMETRUM_DISARM_CMD = 0x04,
    FORCE_FIRST_STAGE_CMD = 0xA5,
    FORCE_SECOND_STAGE_CMD = 0x5A,
} recovery_commands_e;

extern i2c_slave_t i2c;

esp_err_t init_commands(void);

#endif
