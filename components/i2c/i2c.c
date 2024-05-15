#include <i2c.h>

static const char *TAG = "I2C"; 

uint8_t tx_buffer[2];
uint32_t rx_buffer[2];


uint8_t I2C_slave_init(){
    ESP_LOGI(TAG,"I2C INIT");
    i2c_config_t i2c_slave_config = {

        .mode = I2C_MODE_SLAVE,
        .sda_io_num = I2C_SDA,
        .scl_io_num = I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .slave.addr_10bit_en = 0,
        .slave.slave_addr = DEV_ADR,
        .slave.maximum_speed = I2C_SLAVE_MAX_SPEED,
        .clk_flags = 0,
    };
    ESP_LOGI(TAG,"DUPA");
    i2c_param_config(I2C_SLAVE_PORT,&i2c_slave_config);
    ESP_LOGI(TAG,"DUPA2");
    ESP_ERROR_CHECK(i2c_driver_install(I2C_SLAVE_PORT, I2C_MODE_SLAVE, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0 ));
    ESP_LOGI(TAG,"DUPA3 ");
    return RET_SUCCESS;
}

uint8_t I2C_buffer_write(){

    memcpy(&data_to_send,tx_buffer,sizeof(tx_buffer));
    i2c_slave_write_buffer(I2C_SLAVE_PORT, tx_buffer, DATA_SIZE_TO_MCB,100 / portTICK_PERIOD_MS);
    return RET_SUCCESS;
}

uint8_t I2C_buffer_read(){

    i2c_slave_read_buffer(I2C_SLAVE_PORT, rx_buffer, DATA_SIZE_FROM_MCB,100 / portTICK_PERIOD_MS);
    return RET_SUCCESS;

}
