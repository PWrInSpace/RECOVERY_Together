#include "recovery.h"

static const char* TAG = "RECOVERY";

esp_err_t recovery_init(const recovery_config_t *config, recovery_t *recovery) {
    ESP_LOGI(TAG, "Recovery System Initialization");

    if (config->cots_buffer == NULL && config->cots_count > 0) {
        ESP_LOGE(TAG, "Buffer for COTS not provided!");
        return ESP_FAIL;
    }

    *recovery = (recovery_t){
        .config = *config,
        .data = {
            .cots = config->cots_buffer,
            .first_stage_continuity = false,
            .second_stage_continuity = false,
            .separation_1 = false,
            .separation_2 = false,
        },
    };

    const gpio_config_t separation_1_input = {
        .pin_bit_mask = 1ULL << recovery->config.separation_1_pin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    if (gpio_config(&separation_1_input) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure separation_1_pin");
        return ESP_FAIL;
    }

    const gpio_config_t separation_2_input = {
        .pin_bit_mask = 1ULL << recovery->config.separation_2_pin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    if (gpio_config(&separation_2_input) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure separation_2_pin");
        return ESP_FAIL;
    }

    const gpio_config_t first_stage_output = {
        .pin_bit_mask = 1ULL << recovery->config.first_stage_pin,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    if (gpio_config(&first_stage_output) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure first_stage_pin");
        return ESP_FAIL;
    }

    const gpio_config_t second_stage_output = {
        .pin_bit_mask = 1ULL << recovery->config.second_stage_pin,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    if (gpio_config(&second_stage_output) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure second_stage_pin");
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
