#include "i2c.h"

static const char* TAG = "I2C";

esp_err_t i2c_slave_init(const i2c_slave_config_t *config) {
    ESP_LOGI(TAG, "I2C SLAVE INIT (Port: %d, Addr: 0x%02X)", config->port, config->slave_addr);

    const i2c_config_t i2c_slave_config = {
        .mode = I2C_MODE_SLAVE,
        .sda_io_num = config->sda_pin,
        .scl_io_num = config->scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .slave.addr_10bit_en = 0,
        .slave.slave_addr = config->slave_addr,
        .clk_flags = 0,
    };
    
    if (i2c_param_config(config->port, &i2c_slave_config) != ESP_OK) {
        ESP_LOGE(TAG, "I2C param config failed");
        return ESP_FAIL;
    }
    
    if (i2c_driver_install(config->port, I2C_MODE_SLAVE, config->rx_buffer_size, config->tx_buffer_size, 0) != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver install failed");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t i2c_slave_buffer_write(const i2c_port_t i2c_num, const void *data, const int size) {
    i2c_reset_tx_fifo(i2c_num);
    if (i2c_slave_write_buffer(i2c_num, data, size, pdMS_TO_TICKS(1000)) != ESP_OK) {
        ESP_LOGE(TAG, "I2C write buffer failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

int i2c_slave_buffer_read(const i2c_port_t i2c_num, void *data, const size_t size) {
    const int read_bytes = i2c_slave_read_buffer(i2c_num, data, size, pdMS_TO_TICKS(1000));
    if (read_bytes == ESP_FAIL) {
        ESP_LOGE(TAG, "I2C read buffer failed");
    } else if (read_bytes == 0) {
        ESP_LOGW(TAG, "I2C read timeout or empty buffer");
    }
    i2c_reset_rx_fifo(i2c_num);
    return read_bytes;
}
