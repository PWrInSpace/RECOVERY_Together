#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H

#include "spi.h"

#define MISO_PIN 32
#define MOSI_PIN 25
#define SCK_PIN 33
#define SPI_HOST SPI2_HOST

const spi_config_t spi_config = {
    .host_device = SPI_HOST,
    .mosi_pin = MOSI_PIN,
    .miso_pin = MISO_PIN,
    .sck_pin = SCK_PIN
};

#endif //SPI_CONFIG_H