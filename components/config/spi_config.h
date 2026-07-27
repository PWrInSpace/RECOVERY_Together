#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H

#include "spi.h"

static const spi_config_t spi_config = {
    .host_device = SPI2_HOST,
    .mosi_pin = 25U,
    .miso_pin = 32U,
    .sck_pin = 33U
};

#endif