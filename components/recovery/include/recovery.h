#ifndef RECOVERY_CONTROL_H
#define RECOVERY_CONTROL_H

#include "cots.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define TAG "RECOVERY"

#define COTS_COUNT 1
#define SECOND 1000000
#define RESISTANCE_BURN_TIME_MS 7500
#define SERVO_OPEN_TIME_MS 5000

typedef esp_err_t (*first_stage_fnc)(void);
typedef esp_err_t (*second_stage_fnc)(void);

typedef struct{
    gpio_num_t separation_1_pin;
    gpio_num_t separation_2_pin;
    gpio_num_t first_stage_pin;
    gpio_num_t second_stage_pin;
    first_stage_fnc first_stage;
    second_stage_fnc second_stage;
} recovery_config_t;

typedef struct {
    cots_data_t cots[COTS_COUNT];
    bool first_stage_continuity: 1;
    bool second_stage_continuity: 1;
    bool separation_1: 1;
    bool separation_2: 1;
} recovery_data_t;

typedef struct {
    recovery_config_t config;
    recovery_data_t data;
} recovery_t;

esp_err_t recovery_init(const recovery_config_t *config, recovery_t *recovery);

esp_err_t first_stage_deploy(const recovery_t *recovery);

esp_err_t second_stage_deploy(const recovery_t *recovery);

// void check_continuity();
//
// void tele_apogee_isr_handler(void *args);
//
// void easy_apogee_isr_handler(void *args);
//
// void tele_main_isr_handler(void *args);
//
// void turn_off_resistance_timer(void* arg);
//
// void setup_resistance_timer();

#endif