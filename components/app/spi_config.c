#include "spi_config.h"

const static char* TAG = "SPI CONFIG";

static spi_config_t spi_config = {
    .host_device = SPI2_HOST,
    .mosi_pin = 23,
    .miso_pin = 19,
    .sck_pin = 18
};

static StaticSemaphore_t spi_mutex_buffer;

esp_err_t init_spi(void) {
    if (spi_init(&spi_config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI");
        return ESP_FAIL;
    }

    spi_mutex = xSemaphoreCreateMutexStatic(&spi_mutex_buffer);

    return ESP_OK;
}
