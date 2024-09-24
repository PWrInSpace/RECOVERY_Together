#include "recovery_control.h"

static const char *TAG = "RECOVERY";

recovery_device_t recovery_system = {
    .firstStageDone = 0,
    .secondStageDone = 0,
    .easyIgniterCont = 0,
    .teleIgniterCont = 0,
    .endCone = 0,
    .easySecondStage = 0,
    .teleSecondStage = 0,
    .secondStageCont = 0,
    .apogeeDetection = 0
};

uint8_t recovery_Init(){

    ESP_LOGI(TAG,"Recovery System Initialization");

    gpio_config_t gpio_inputs = {
        .pin_bit_mask = ((1ULL << END_CONE)),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config_t gpio_outputs = {
        .pin_bit_mask = ((1ULL << PILOT_DEPLOY) | (1ULL << EASY_IGNITER_FIRE) | (1ULL << LED) |
                         (1ULL << TELE_IGNITER_FIRE) | (1ULL << EASY_ARMING) | (1ULL << TELE_ARMING)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };


    ESP_ERROR_CHECK(gpio_config(&gpio_inputs));

    ESP_ERROR_CHECK(gpio_config(&gpio_outputs));


    recovery_system.endconePin = END_CONE;
    recovery_system.pilotDeployPin = PILOT_DEPLOY;
    recovery_system.easyIgniterContPin = EASY_IGNITER_CONT;
    recovery_system.teleIgniterContPin = TELE_IGNITER_CONT;
    recovery_system.easyIgniterFirePin = EASY_IGNITER_FIRE;
    recovery_system.teleIgniterFirePin = TELE_IGNITER_FIRE;

    ESP_LOGI(TAG,"Recovery system initialization done :D");
    return RET_SUCCESS;
}

uint8_t first_Stage_Deploy(){

   // ESP_LOGI(TAG,"First stage deploy event");

    gpio_set_level(recovery_system.pilotDeployPin,1);

    //while(!gpio_get_level(recovery_system.endconePin)){

    //    gpio_set_level(recovery_system.pilotDeployPin,1);

      //  ESP_LOGW(TAG,"No confirmation for first stage deploy!!!");

    //}
    recovery_system.firstStageDone = true;

    ESP_LOGI(TAG,"Recovery first stage done");

    return RET_SUCCESS;

}

uint8_t second_Stage_Deploy(){

    ESP_LOGI(TAG,"Second stage deploy event");

    gpio_set_level(recovery_system.easyIgniterFirePin,1);
    gpio_set_level(recovery_system.teleIgniterFirePin,1);

    // while(!gpio_get_level(recovery_system.easyIgniterContPin) && !gpio_get_level(recovery_system.teleIgniterContPin)){
    //     gpio_set_level(recovery_system.easyIgniterFirePin,1);
    //     gpio_set_level(recovery_system.teleIgniterFirePin,1);
        
    //     ESP_LOGW(TAG,"No confirmation for second stage deploy!!!");
    // }

    recovery_system.secondStageDone = true;

    ESP_LOGI(TAG,"Second stage recovery done");

    return RET_SUCCESS;

}

void check_Cont(){


    bool previous_easymini_igniter_cont = recovery_system.easyIgniterCont;
    bool previous_telemetrum_igniter_cont = recovery_system.teleIgniterCont;


    if(!gpio_get_level(recovery_system.teleIgniterContPin)) recovery_system.teleIgniterCont = true;
    else recovery_system.teleIgniterCont = false;

    if(!gpio_get_level(recovery_system.easyIgniterContPin)) {recovery_system.easyIgniterCont = true;
    }
    else {recovery_system.easyIgniterCont = false;
    }

    if(previous_easymini_igniter_cont == 1 && recovery_system.easyIgniterCont == 0){
        recovery_system.secondStageDone = true;
        recovery_system.easySecondStage = true;
    }
    else if(previous_telemetrum_igniter_cont == 1 && recovery_system.teleIgniterCont == 0){
        recovery_system.secondStageDone = true;
        recovery_system.teleSecondStage = true;
    }

}

void tele_apogee_isr_handler(void *args){

    telemetrum_device.apogeeDetection = 1;
    gpio_set_level(PILOT_DEPLOY,1);
    recovery_system.firstStageDone = 1;

}

void easy_apogee_isr_handler(void *args){

    easymini_device.apogeeDetection = 1;
    gpio_set_level(PILOT_DEPLOY,1);
    recovery_system.firstStageDone = 1;

}
