#include "i2c.h"

const static char *TAG = "I2C";

static bool on_recv_done_callback(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_rx_done_event_data_t *evt_data, void *arg) {
    const i2c_slave_t *slave = arg;

    if (slave == NULL || slave->config.receive_callback == NULL) {
        ESP_LOGE(TAG, "Invalid arguments");
        return false;
    }

    return slave->config.receive_callback(evt_data->buffer);
}

esp_err_t i2c_init(const sys_i2c_config_t *i2c_config, i2c_slave_t *i2c_slave) {
    i2c_slave->config = *i2c_config;

    const i2c_slave_config_t config = {
        .addr_bit_len = I2C_ADDR_BIT_LEN_7,
        .i2c_port = i2c_config->port,
        .send_buf_depth = i2c_config->tx_buffer_size,
        .scl_io_num = i2c_config->scl_pin,
        .sda_io_num = i2c_config->sda_pin,
        .slave_addr = i2c_config->slave_addr,
    };

    if (i2c_new_slave_device(&config, &i2c_slave->slave_handle) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create I2C slave device");
        return ESP_FAIL;
    }

    const i2c_slave_event_callbacks_t callbacks = {
        .on_recv_done = on_recv_done_callback,
    };

    if (i2c_slave_register_event_callbacks(i2c_slave->slave_handle, &callbacks, i2c_slave) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register I2C slave event callbacks");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t i2c_deinit(const i2c_slave_t *i2c_slave) {
    if (i2c_del_slave_device(i2c_slave->slave_handle) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to delete I2C slave device");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t i2c_write(const i2c_slave_t *i2c_slave, const uint8_t *data, const int data_size) {
    if (data_size > i2c_slave->config.tx_buffer_size) {
        ESP_LOGE(TAG, "Data size exceeds TX buffer size");
        return ESP_FAIL;
    }


    if (i2c_slave_transmit(i2c_slave->slave_handle, data, data_size, 100) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write data to I2C slave");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t i2c_read(const i2c_slave_t *i2c_slave) {
    if (i2c_slave_receive(i2c_slave->slave_handle, i2c_slave->config.rx_buffer, i2c_slave->config.rx_buffer_size) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start I2C receive");
        return ESP_FAIL;
    }

    return ESP_OK;
}
