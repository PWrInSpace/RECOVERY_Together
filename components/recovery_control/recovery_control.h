#ifndef RECOVERY_CONTROL_H
#define RECOVERY_CONTROL_H

#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "config.h"
#include "pinout.h"

typedef struct{

    gpio_num_t endconePin;
    gpio_num_t pilotDeployPin;
    gpio_num_t easyIgniterContPin;
    gpio_num_t teleIgniterContPin;
    gpio_num_t easyIgniterFirePin;
    gpio_num_t teleIgniterFirePin;
    bool firstStageDone : 1;
    bool secondStageDone : 1;
    bool easyIgniterCont : 1;
    bool teleIgniterCont : 1;
    bool endCone : 1;
    bool easySecondStage : 1;
    bool teleSecondStage : 1;
    bool secondStageCont : 1;

}recovery_device_t;

extern recovery_device_t recovery_system;

uint8_t recovery_Init();
uint8_t first_Stage_Deploy();
uint8_t second_Stage_Deploy();
void check_Cont();
void apogee_isr_handler(void *args);

#endif