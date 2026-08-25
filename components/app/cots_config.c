#include "cots_config.h"

static const char *TAG = "COTS CONFIG";

cots_t telemetrum;
cots_t easymini;

static cots_config_t easymini_config = {
    .arming_pin = GPIO_NUM_26,
    .apogee_pin = GPIO_NUM_27,
    .main_pin = GPIO_NUM_14,
};

static cots_config_t telemetrum_config = {
    .arming_pin = GPIO_NUM_29,
    .apogee_pin = GPIO_NUM_28,
    .main_pin = GPIO_NUM_16,
};

esp_err_t init_telemetrum(void) {
    telemetrum_config.recovery = &recovery.config;
    if (cots_init(&telemetrum_config, &telemetrum) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize telemetrum");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t init_easymini(void) {
    easymini_config.recovery = &recovery.config;
    if (cots_init(&easymini_config, &easymini) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize easymini");
        return ESP_FAIL;
    }
    return ESP_OK;
}
