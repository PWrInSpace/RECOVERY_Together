#ifndef RECOVERY_CONTROL_H
#define RECOVERY_CONTROL_H

#include <stdio.h>
#include  "driver/gpio.h"
#include "esp_log.h"
#include "config.h"

typedef struct{

    gpio_num_t endconePin;
    gpio_num_t pilotDeployPin;
    gpio_num_t easyIgniterContPin;
    gpio_num_t teleIgniterContPin;
    gpio_num_t easyIgniterFirePin;
    gpio_num_t teleIgniterFirePin;
    bool firstStageDone;
    bool secondStageDone;
    bool easyIgniterCont;
    bool teleIgniterCont;
    bool endCone;

}Recovery_system_t

uint8_t recovery_Init(Recovery_system_t *recovery_ptr);
uint8_t first_Stage_Deploy(Recovery_system_t *recovery_ptr);
uint8_t second_Stage_Deploy(Recovery_system_t *recovery_ptr);
void check_Cont(Recovery_system_t *recovery_ptr);

#endif