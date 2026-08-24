#include "recovery.h"

static const char* TAG = "RECOVERY";

esp_err_t recovery_init(const recovery_config_t *config, recovery_t *recovery) {
    ESP_LOGI(TAG, "Recovery System Initialization");

    *recovery = (recovery_t){
        .config = *config,
        .data = {
            .separation_1 = false,
            .separation_2 = false,
        },
    };

    const gpio_config_t gpio_separation_inputs = {
        .pin_bit_mask = (1ULL << recovery->config.separation_1_pin) | (1ULL << recovery->config.separation_2_pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE,
    };
    if (gpio_config(&gpio_separation_inputs) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure separation pins");
        return ESP_FAIL;
    }

    const gpio_config_t gpio_stage_inputs = {
        .pin_bit_mask = (1ULL << recovery->config.first_stage_pin) | (1ULL << recovery->config.second_stage_pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    if (gpio_config(&gpio_stage_inputs) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure stage pins");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Recovery system initialization done :D");
    return ESP_OK;
}

esp_err_t first_stage_deploy(const recovery_t *recovery) {
    if (recovery->config.first_stage() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to deploy first stage");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t second_stage_deploy(const recovery_t *recovery){
    if (recovery->config.second_stage() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to deploy second stage");
        return ESP_FAIL;
    }

    return ESP_OK;
}
