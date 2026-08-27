#include "recovery.h"

#define SEPARATION_TIMER_PERIOD_US 10000

static const char* TAG = "RECOVERY";

static void on_separation_one_timer(void *arg) {
    recovery_t *recovery = arg;
    if (!gpio_get_level(recovery->config.separation_one_pin)) {
        // ESP_LOGI(TAG, "Separation one detected");
        recovery->data.separation_one = true;
    }
}

static void on_separation_two_timer(void *arg) {
    recovery_t *recovery = arg;
    if (!gpio_get_level(recovery->config.separation_two_pin)) {
        // ESP_LOGI(TAG, "Separation two detected");
        recovery->data.separation_two = true;
    }
}

esp_err_t recovery_init(const recovery_config_t *config, recovery_t *recovery) {
    ESP_LOGI(TAG, "Recovery System Initialization");

    *recovery = (recovery_t){
        .config = *config,
        .data = {
            .separation_one = false,
            .separation_two = false,
        },
        .separation_one_timer = NULL,
        .separation_two_timer = NULL,
    };

    const gpio_config_t gpio_separation_inputs = {
        .pin_bit_mask = (1ULL << recovery->config.separation_one_pin) | (1ULL << recovery->config.separation_two_pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    if (gpio_config(&gpio_separation_inputs) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure separation pins");
        return ESP_FAIL;
    }

    const gpio_config_t gpio_stage_outputs = {
        .pin_bit_mask = (1ULL << recovery->config.first_stage_pin) | (1ULL << recovery->config.second_stage_pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    if (gpio_config(&gpio_stage_outputs) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure stage pins");
        return ESP_FAIL;
    }

    if (gpio_install_isr_service(0) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install ISR service");
        return ESP_FAIL;
    }

    esp_timer_create_args_t timer_arg = {
        .arg = &recovery,
        .callback = on_separation_one_timer,
        .name = "separation one timer",
    };
    esp_timer_create(&timer_arg, &recovery->separation_one_timer);
    esp_timer_start_periodic(recovery->separation_one_timer, SEPARATION_TIMER_PERIOD_US);

    timer_arg = (esp_timer_create_args_t){
        .arg = &recovery,
        .callback = on_separation_two_timer,
        .name = "separation two timer",
    };
    esp_timer_create(&timer_arg, &recovery->separation_two_timer);
    esp_timer_start_periodic(recovery->separation_two_timer, SEPARATION_TIMER_PERIOD_US);

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
