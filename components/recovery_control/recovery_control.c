#include "recovery_control.h"

static const char *TAG = "RECOVERY";

recovery_device_t recovery_system;

uint8_t recovery_Init(){

    ESP_LOGI(TAG,"Recovery System Initialization");

    gpio_config_t gpio_inputs = {
        .pin_bit_mask = ((1ULL << END_CONE) | (1ULL << TELE_IGNITER_CONT) | (1ULL << EASY_IGNITER_CONT)),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config_t gpio_outputs = {
        .pin_bit_mask = ((1ULL << PILOT_DEPLOY) | (1ULL << TELE_IGNITER_FIRE) | (1ULL << EASY_IGNITER_FIRE)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&gpio_inputs));
    ESP_ERROR_CHECK(gpio_config(&gpio_outputs));

    recovery_system.endconePin = END_CONE;
    recovery_system.pilotDeployPin = PILOT_DEPLOY;
    recovery_system.easyIgniterContPin = TELE_IGNITER_CONT;
    recovery_system.teleIgniterContPin = EASY_IGNITER_CONT;
    recovery_system.easyIgniterFirePin = EASY_IGNITER_FIRE;
    recovery_system.teleIgniterFirePin = TELE_IGNITER_FIRE;

    ESP_LOGI(TAG,"Recovery system initialization done :D");
    return RET_SUCCESS;
}

uint8_t first_Stage_Deploy(){

    ESP_LOGI(TAG,"First stage deploy event");

    gpio_set_level(recovery_system.pilotDeployPin,1);

    while(!gpio_get_level(recovery_system.endconePin)){

        gpio_set_level(recovery_system.pilotDeployPin,1);

        ESP_LOGW(TAG,"No confirmation for first stage deploy!!!");

    }
    recovery_system.firstStageDone = true;

    ESP_LOGI(TAG,"Recovery first stage done");

    return RET_SUCCESS;

}

uint8_t second_Stage_Deploy(){

    ESP_LOGI(TAG,"Second stage deploy event");

    gpio_set_level(recovery_system.easyIgniterFirePin,1);
    gpio_set_level(recovery_system.teleIgniterFirePin,1);

    while(!gpio_get_level(recovery_system.easyIgniterContPin) && !gpio_get_level(recovery_system.teleIgniterContPin)){
        gpio_set_level(recovery_system.easyIgniterFirePin,1);
        gpio_set_level(recovery_system.teleIgniterFirePin,1);
        
        ESP_LOGW(TAG,"No confirmation for second stage deploy!!!");
    }

    recovery_system.secondStageDone = true;

    ESP_LOGI(TAG,"Second stage recovery done");

    return RET_SUCCESS;

}

void check_Cont(){

    ESP_LOGI(TAG,"Checking continuinty");

    if(gpio_get_level(recovery_system.endconePin)) recovery_system.endCone = true;
    else recovery_system.endCone = false;

    if(gpio_get_level(recovery_system.easyIgniterContPin)) recovery_system.easyIgniterCont = true;
    else recovery_system.easyIgniterCont = false;

    if(gpio_get_level(recovery_system.teleIgniterContPin)) recovery_system.teleIgniterCont = true;
    else recovery_system.teleIgniterCont = false;

    ESP_LOGI(TAG,"Checking continuity done");

}

void apogee_isr_handler(void *args){

    ESP_LOGI(TAG,"APOGEE DETECTED !!!");
    ESP_LOGI(TAG,"DEPLOYING PILOT PARACHUTE");

    first_Stage_Deploy();

}
