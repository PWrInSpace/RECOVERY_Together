#include "recovery_config.h"

#define COTS_COUNT 1
#define RESISTANCE_BURN_TIME_US 7500000

static const char *TAG = "RECOVERY CONFIG";

recovery_t recovery;

static esp_timer_handle_t resistance_wire_off_timer;

esp_err_t resistance_wire_on_time(const int time) {
    if (esp_timer_start_once(resistance_wire_off_timer, time) != ESP_OK) {
        return ESP_FAIL;
    }

    gpio_set_level(recovery.config.first_stage_pin, 1);

    return ESP_OK;
}

static esp_err_t first_stage_callback(void) {
    resistance_wire_on_time(RESISTANCE_BURN_TIME_US);

    recovery.data.separation_1 = true;

    return ESP_OK;
}

static esp_err_t second_stage_callback(void) {
    if (servo_open(&servo) != ESP_OK) {
        return ESP_FAIL;
    }

    recovery.data.separation_2 = true;

    return ESP_OK;
}

static void turn_off_resistance_timer(void *arg) {
    gpio_set_level(recovery.config.first_stage_pin, 0);
}

static esp_err_t setup_resistance_timer() {
    const esp_timer_create_args_t timer_args = {
        .callback = turn_off_resistance_timer,
        .name = "resistance_off_timer"
    };

    if (esp_timer_create(&timer_args, &resistance_wire_off_timer) != ESP_OK) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

static recovery_config_t recovery_config = {
    .separation_1_pin = GPIO_NUM_4,
    .separation_2_pin = GPIO_NUM_5,
    .first_stage_pin = GPIO_NUM_18,
    .second_stage_pin = GPIO_NUM_19,
    .first_stage = first_stage_callback,
    .second_stage = second_stage_callback
};

esp_err_t init_recovery() {
    if (recovery_init(&recovery_config, &recovery) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize recovery");
        return ESP_FAIL;
    }

    if (setup_resistance_timer() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to setup resistance timer");
        return ESP_FAIL;
    }

    return ESP_OK;
}

