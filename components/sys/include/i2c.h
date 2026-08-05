#ifndef I2C_H
#define I2C_H

#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_err.h"

typedef struct {
    i2c_port_t port;
    uint8_t sda_pin;
    uint8_t scl_pin;
    uint16_t slave_addr;
    size_t rx_buffer_size;
    size_t tx_buffer_size;
} i2c_slave_config_t;

esp_err_t i2c_slave_init(const i2c_slave_config_t *config);

esp_err_t i2c_slave_buffer_write(i2c_port_t i2c_num, const void *data, int size);

int i2c_slave_buffer_read(i2c_port_t i2c_num, void *data, size_t size);

#endif