#ifndef I2C_H
#define I2C_H

#include "driver/i2c.h"
#include "esp_log.h"
#include "config.h"
#include "dataStructs.h"
#include "pinout.h"
#include "string.h"

typedef union {
    struct command {
        uint32_t command;
        int32_t payload;
    } cmd;

    uint8_t raw[sizeof(struct command)];
} cmd_message_t;

extern uint8_t tx_buffer[4];
extern cmd_message_t rx_buffer;

uint8_t I2C_slave_init();
uint8_t I2C_buffer_write();
uint8_t I2C_buffer_read();

#endif