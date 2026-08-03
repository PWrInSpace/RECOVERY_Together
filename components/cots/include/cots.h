#ifndef COTS_H
#define COTS_H 

#include "driver/gpio.h"
#include "esp_log.h"

#define DISARMED 0
#define ARMED 1

typedef struct {
    gpio_num_t arming_pin;
    gpio_num_t apogee_pin;
    gpio_num_t main_pin;
} cots_config_t;

typedef struct {
    bool armed: 1;
    bool apogee_detected: 1;
    bool first_stage: 1;
    bool second_stage: 1;
} cots_data_t;

typedef struct {
    cots_config_t config;
    cots_data_t data;
} cots_t;

esp_err_t cots_init(const cots_config_t *cots_config, cots_t *cots);

esp_err_t cots_arm(cots_t *cots);

esp_err_t cots_disarm(cots_t *cots);

esp_err_t apogee_check(cots_t *cots);

#endif // COTS_H

