#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H

#include "spi.h"
#include "esp_log.h"

extern SemaphoreHandle_t spi_mutex;

esp_err_t init_spi(void);

#endif
