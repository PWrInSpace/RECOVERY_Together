#ifndef RECOVERY_CONTROL_H
#define RECOVERY_CONTROL_H

#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "config.h"
#include "pinout.h"
#include "servo_control.h"
#include "cots.h"
#include "esp_timer.h"

#define RESISTANCE_BURN_TIME_US 5000000
#define SERVO_OPEN_TIME_US      5000000

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
    bool apogeeDetection : 1;

}recovery_device_t;

extern recovery_device_t recovery_system;
extern esp_timer_handle_t resistance_off_timer;

uint8_t recovery_Init();
uint8_t first_Stage_Deploy();
uint8_t second_Stage_Deploy();
void check_Cont();
void tele_apogee_isr_handler(void *args);
void easy_apogee_isr_handler(void *args);
void turn_off_resistance_timer(void* arg);
void setup_resistance_timer();

#endif