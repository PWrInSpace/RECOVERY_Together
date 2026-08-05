#include "servo_config.h"

servo_control_t servo;

static const char *TAG = "SERVO CONFIG";

static servo_config_t servo_config = {
    .servo_pin = GPIO_NUM_18,
    .max_pulse_width_us = 500,
    .min_pulse_width_us = 2500,
    .max_angle = 0,
    .min_angle = 180,
    .frequency_hz = 1000000,
    .timebase_period = 20000,
    .open_angle = 85U,
    .close_angle = 120U
};

esp_err_t init_servo(void) {
    if (servo_init(&servo_config, &servo) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize servo");
        return ESP_FAIL;
    }
    return ESP_OK;
}