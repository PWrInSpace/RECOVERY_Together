#ifndef I2C_H
#define I2C_H

#include "driver/i2c.h"
#include "esp_log.h"
#include "config.h"
#include "dataStructs.h"
#include "pinout.h"
#include "string.h"


extern uint8_t tx_buffer[2];
extern uint32_t rx_buffer[2];

uint8_t I2C_slave_init();
uint8_t I2C_buffer_write();
uint8_t I2C_buffer_read();

#endif