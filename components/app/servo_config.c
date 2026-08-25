#include "servo_config.h"

#define SERVO_PIN GPIO_NUM_17
#define MAX_PULSE_WIDTH_US 500
#define MIN_PULSE_WIDTH_US 2500
#define MAX_ANGLE 0
#define MIN_ANGLE 180
#define FREQUENCY_HZ 1000000
#define TIMEBASE_PERIOD 20000
#define OPEN_ANGLE 85U
#define CLOSE_ANGLE 120U

static const char *TAG = "SERVO CONFIG";

servo_control_t servo;

static servo_config_t servo_config = {
    .servo_pin = SERVO_PIN,
    .max_pulse_width_us = MAX_PULSE_WIDTH_US,
    .min_pulse_width_us = MIN_PULSE_WIDTH_US,
    .max_angle = MAX_ANGLE,
    .min_angle = MIN_ANGLE,
    .frequency_hz = FREQUENCY_HZ,
    .timebase_period = TIMEBASE_PERIOD,
    .open_angle = OPEN_ANGLE,
    .close_angle = CLOSE_ANGLE
};

esp_err_t init_servo(void) {
    if (servo_init(&servo_config, &servo) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize servo");
        return ESP_FAIL;
    }
    return ESP_OK;
}