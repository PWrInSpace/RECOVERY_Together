#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include "driver/mcpwm_prelude.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

typedef struct {
    gpio_num_t servo_pin;
    uint16_t max_pulse_width_us;
    uint16_t min_pulse_width_us;
    uint16_t max_angle;
    uint16_t min_angle;
    uint32_t frequency_hz;
    uint32_t timebase_period;
    uint16_t open_angle;
    uint16_t close_angle;
} servo_config_t;

typedef struct {
    servo_config_t config;
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t oper;
    mcpwm_gen_handle_t generator;
    mcpwm_cmpr_handle_t comparator;
    int angle;
    esp_timer_handle_t auto_close_timer;
} servo_control_t;

uint32_t angle_to_compare(const servo_control_t *servo, int angle);

esp_err_t servo_init(const servo_config_t *config, servo_control_t *servo);

esp_err_t servo_set_angle(servo_control_t *servo, int angle);

esp_err_t servo_open(servo_control_t *servo);

esp_err_t servo_close(servo_control_t *servo);

esp_err_t servo_open_for(servo_control_t *servo, uint64_t duration_us);

#endif
