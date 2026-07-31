#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H

#include "spi.h"
#include "hal/gpio_types.h"

static const spi_config_t spi_config = {
    .host_device = SPI2_HOST,
    .mosi_pin = GPIO_NUM_25,
    .miso_pin = GPIO_NUM_32,
    .sck_pin = GPIO_NUM_33
};

#endif