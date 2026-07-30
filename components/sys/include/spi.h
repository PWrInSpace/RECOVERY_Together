#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "esp_err.h"
#include "driver/spi_master.h"

#define TAG "SPI"

typedef struct {
    spi_host_device_t host_device;
    uint8_t mosi_pin;
    uint8_t miso_pin;
    uint8_t sck_pin;
} spi_config_t;

esp_err_t spi_init(const spi_config_t *spi_config);

#endif
