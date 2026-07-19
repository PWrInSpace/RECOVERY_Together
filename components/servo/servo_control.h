#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include "driver/mcpwm_prelude.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "config.h"
#include "pinout.h"
#include "esp_timer.h"

#define SERVO_MIN_PULSEWIDTH_US 500   // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2500  // Maximum pulse width in microsecond
#define SERVO_MIN_ANGLE 0             // Minimum angle
#define SERVO_MAX_ANGLE 180           // Maximum angle
#define SERVO_FREQUENCY_HZ 1000000    // Frequency of the PWM signal for the servo
#define SERVO_TIMEBASE_PERIOD 20000   // 20000 ticks, 20ms

#define SERVO_CLOSED 85U
#define SERVO_OPENED 120U

#define TAG "SERVO"

typedef struct {
    gpio_num_t servo_pin;
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t oper;
    mcpwm_gen_handle_t generator;
    mcpwm_cmpr_handle_t comparator;
    int angle;
} servo_control_t;

uint32_t angle_to_compare(int angle);
void servo_init(servo_control_t *servo);
void servo_set_angle(servo_control_t *servo, int angle);
void servo_open(servo_control_t *servo);
void servo_close(servo_control_t *servo);
void servo_open_for(servo_control_t *servo, uint64_t duration_us);

#endif
