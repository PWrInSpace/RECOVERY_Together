#include "recovery.h"

static const char* TAG = "RECOVERY";

esp_err_t recovery_init(const recovery_config_t *config, recovery_t *recovery) {
    ESP_LOGI(TAG, "Recovery System Initialization");

    recovery->config = *config;

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

    // setup_resistance_timer();

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

// void check_continuity(){
//
//     bool previous_easymini_igniter_cont = recovery_system.easyIgniterCont;
//     bool previous_telemetrum_igniter_cont = recovery_system.teleIgniterCont;
//
//     recovery_system.teleIgniterCont = !gpio_get_level(recovery_system.teleIgniterContPin);
//     recovery_system.easyIgniterCont = !gpio_get_level(recovery_system.easyIgniterContPin);
//
//     if(previous_easymini_igniter_cont == 1 && recovery_system.easyIgniterCont == 0){
//         recovery_system.second_stage = true;
//         recovery_system.easySecondStage = true;
//     }
//     else if(previous_telemetrum_igniter_cont == 1 && recovery_system.teleIgniterCont == 0){
//         recovery_system.second_stage = true;
//         recovery_system.teleSecondStage = true;
//     }
// }

// void tele_apogee_isr_handler(void *args) {
//     telemetrum_device.apogee_detected = 1;
//     first_stage_deploy();
// }
//
// void easy_apogee_isr_handler(void *args) {
//     easymini_device.apogee_detected = 1;
//     first_stage_deploy();
// }
//
// void tele_main_isr_handler(void *args) {
//     second_stage_deploy();
// }
//
// void turn_off_resistance_timer(void* arg) {
//     gpio_set_level(recovery_system.pilot_deploy_pin, 0);
//     ESP_EARLY_LOGW(TAG, "Resistance wire turned off");
// }

// void setup_resistance_timer() {
//
//     const esp_timer_create_args_t timer_args = {
//         .callback = &turn_off_resistance_timer,
//         .name = "resistance_off_timer"
//     };
//     esp_timer_create(&timer_args, &resistance_off_timer);
//
// }
