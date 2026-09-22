#ifndef I2C_H
#define I2C_H

#include "driver/i2c_slave.h"
#include "esp_log.h"

typedef bool (*i2c_receive_callback)(const uint8_t *data);

typedef struct {
    int port;
    uint8_t sda_pin;
    uint8_t scl_pin;
    uint16_t slave_addr;
    size_t tx_buffer_size;
    size_t rx_buffer_size;
    uint8_t *rx_buffer;
    i2c_receive_callback receive_callback;
} sys_i2c_config_t;

typedef struct {
    sys_i2c_config_t config;
    i2c_slave_dev_handle_t slave_handle;
} i2c_slave_t;

esp_err_t i2c_init(const sys_i2c_config_t *i2c_config, i2c_slave_t *i2c_slave);

esp_err_t i2c_deinit(const i2c_slave_t *i2c_slave);

esp_err_t i2c_write(const i2c_slave_t *i2c_slave, const uint8_t *data, int data_size);

esp_err_t i2c_read(const i2c_slave_t *i2c_slave);

#endif