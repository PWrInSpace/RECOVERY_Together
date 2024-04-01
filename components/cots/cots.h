#ifndef COTS_H
#define COTS_H 

#include <stdio.h>
#include  "driver/gpio.h"
#include "esp_log.h"
#include "config.h"

typedef enum{

    COTS_DEVICE_TELEMETRUM = 0,
    COTS_DEVICE_EASYMINI,
    
}cots_device_t

typedef struct {

    // Arming control and status
    bool armStatus;
    gpio_num_t armingPin;

    // GPIOs for detecting apogee and main deploy atitude
    bool apogeeDetection; // Probably useless 
    gpio_num_t apogeePin;
    gpio_num_t igniterPin;


} cots_struct_t;


uint8_t cots_init(cots_device_t cots_device);
uint8_t cots_arming(cots_device_t cots_device);
uint8_t cots_disarm(cots_device_t cots_device);
uint8_t apogee_check(cots_device_t cots_device);



#endif // COTS_H

