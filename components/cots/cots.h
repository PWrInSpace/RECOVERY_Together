#ifndef COTS_H
#define COTS_H 

#include <stdio.h>
#include "driver/gpio.h"
#include "config.h"
#include "pinout.h"

typedef struct {
    bool armed;
    bool apogee_detected;
    gpio_num_t arming_pin;
    gpio_num_t apogee_pin;
    gpio_num_t main_pin;
} cots_t;

extern cots_t telemetrum_device;
extern cots_t easymini_device;

esp_err_t cots_init(cots_t *cots);

esp_err_t cots_arming(cots_t *cots);

esp_err_t cots_disarm(cots_t *cots);

esp_err_t apogee_check(cots_t *cots);

#endif // COTS_H

