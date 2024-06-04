#include <i2c.h>

static const char *TAG = "I2C"; 

uint8_t tx_buffer[4];
cmd_message_t rx_buffer;


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
        .clk_flags = 0,
    };  
    i2c_param_config(I2C_SLAVE_PORT,&i2c_slave_config);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_SLAVE_PORT, I2C_MODE_SLAVE, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0 ));
    return RET_SUCCESS;
}

uint8_t I2C_buffer_write(){
    uint8_t bytes;
    memcpy(&tx_buffer,&data_to_send,sizeof(tx_buffer));
    i2c_reset_tx_fifo(I2C_SLAVE_PORT);
    bytes = i2c_slave_write_buffer(I2C_SLAVE_PORT, tx_buffer, DATA_SIZE_TO_MCB,100 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG,"Writed %d bytes into the buffer", bytes);
    ESP_LOGI(TAG, "Buffer : %d, %d, %d, %d", tx_buffer[0],tx_buffer[1],tx_buffer[2],tx_buffer[3]);
    return RET_SUCCESS;
}

uint8_t I2C_buffer_read(){
    uint8_t readBytes;
    readBytes = i2c_slave_read_buffer(I2C_SLAVE_PORT, &rx_buffer.raw[0], DATA_SIZE_FROM_MCB,100 / portTICK_PERIOD_MS);
    i2c_reset_rx_fifo(I2C_SLAVE_PORT);

    ESP_LOGI(TAG,"readed %d bytes",readBytes);


    return RET_SUCCESS;

}
