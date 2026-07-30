#include "spi.h"

esp_err_t spi_init(const spi_config_t *spi_config) {
    spi_bus_config_t bus_config = {0};

    bus_config.mosi_io_num = spi_config->mosi_pin;
    bus_config.miso_io_num = spi_config->miso_pin;
    bus_config.sclk_io_num = spi_config->sck_pin;

    bus_config.quadwp_io_num = -1;
    bus_config.quadhd_io_num = -1;
    bus_config.data4_io_num = -1;
    bus_config.data5_io_num = -1;
    bus_config.data6_io_num = -1;
    bus_config.data7_io_num = -1;

    if (spi_bus_initialize(spi_config->host_device, &bus_config, SPI_DMA_CH_AUTO) != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}
