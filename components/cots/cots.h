#ifndef COTS_H
#define COTS_H 

#include <stdio.h>
#include "driver/gpio.h"
#include "config.h"
#include "pinout.h"

typedef enum{

    COTS_DEVICE_TELEMETRUM = 0,
    COTS_DEVICE_EASYMINI,
    
}cots_device_t;

typedef struct {

    bool armStatus;
    gpio_num_t armingPin;
    gpio_num_t apogeePin;
    gpio_num_t igniterPin;
    bool apogeeDetection;

} cots_struct_t;

extern cots_struct_t telemetrum_device;
extern cots_struct_t easymini_device;

uint8_t cots_init(cots_device_t cots_device);
uint8_t cots_arming(cots_device_t cots_device);
uint8_t cots_disarm(cots_device_t cots_device);
uint8_t apogee_check();



#endif // COTS_H

