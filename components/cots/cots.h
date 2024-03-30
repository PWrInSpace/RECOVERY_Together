#ifndef COTS_H
#define COTS_H 

#include <stdio.h>
#include  "driver/gpio.h"
#include "esp_log.h"
#include "config.h"

typedef struct {

    // Arming control and status
    bool armStatus;
    gpio_num_t armingPin;

    // GPIOs for detecting apogee and main deploy atitude
    bool apogeeDetection;
    gpio_num_t apogeePin;
    gpio_num_t igniterPin;


} Cots_t;


uint8_t cots_init(Cots_t *telemtrum_ptr, Cots_t *easymini_ptr);
uint8_t cots_arming(Cots_t *telemtrum_ptr, Cots_t *easymini_ptr);
uint8_t cots_disarm(Cots_t *telemtrum_ptr, Cots_t *easymini_ptr);
uint8_t apogee_check(Cots_t *telemtrum_ptr, Cots_t *easymini_ptr);



#endif // COTS_H

