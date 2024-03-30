#include "recovery_control.h"

static const char *TAG = "RECOVERY";

uint8_t recovery_Init(Recovery_system_t *recovery_ptr){

    ESP_LOGI(TAG,"Recovery System Initialization");

    gpio_reset_pin(END_CONE);
    gpio_set_direction(END_CONE,GPIO_MODE_INPUT);
    gpio_reset_pin(TELE_IGNITER_CONT);
    gpio_set_direction(TELE_IGNITER_CONT,GPIO_MODE_INPUT);
    gpio_reset_pin(EASY_IGNITER_CONT);
    gpio_set_direction(EASY_IGNITER_CONT,GPIO_MODE_INPUT);
    gpio_reset_pin(PILOT_DEPLOY);
    gpio_set_direction(PILOT_DEPLOY,GPIO_MODE_OUTPUT);
    gpio_reset_pin(TELE_IGNITER_FIRE);
    gpio_set_direction(TELE_IGNITER_FIRE,GPIO_MODE_OUTPUT);
    gpio_reset_pin(EASY_IGNITER_FIRE);
    gpio_set_direction(EASY_IGNITER_FIRE,GPIO_MODE_OUTPUT);

    recovery_ptr->endconePin = END_CONE;
    recovery_ptr->pilotDeployPin = PILOT_DEPLOY;
    recovery_ptr->easyIgniterContPin = EASY_IGNITER_CONT;
    recovery_ptr->teleIgniterFirePin = TELE_IGNITER_CONT;
    recovery_ptr->easyIgniterfirePin = EASY_IGNITER_FIRE;
    recovery_ptr->teleIgniterFirePin = TELE_IGNITER_FIRE;

    return RET_SUCCESS;
}

uint8_t first_Stage_Deploy(Recovery_system_t *recovery_ptr){

    ESP_LOGI(TAG,"First stage deploy event");

    gpio_set_level(recovery_ptr->pilotDeployPin,1);

    while(!gpio_get_level(recovery_ptr->endconePin)){

        gpio_set_level(recovery_ptr->pilotDeployPin,1);

        ESP_LOGW(TAG,"No confirmation for first stage deploy!!!");

    }
    recovery_ptr->firstStageDone = true;
    return RET_SUCCESS;

}

uint8_t second_Stage_Deploy(Recovery_system_t *recovery_ptr){

    ESP_LOGI(TAG,"Second stage deploy event");

    gpio_set_level(recovery_ptr->easyIgniterFirePin,1);
    gpio_set_level(recovery_ptr->teleIgniterFirePin,1);

    while(!gpio_get_level(recovery_ptr->easyIgniterContPin) && !gpio_get_level(recovery_ptr->teleIgniterContPin)){
        gpio_set_level(recovery_ptr->easyIgniterFirePin,1);
        gpio_set_level(recovery_ptr->teleIgniterFirePin,1);
        
        ESP_LOGW(TAG,"No confirmation for second stage deploy!!!");
    }

    recovery_ptr->secondStageDone = true;
    return RET_SUCCESS;

}

void check_Cont(Recovery_system_t *recovery_ptr){

    ESP_LOGI(TAG,"Cecking continuinty");

    if(gpio_get_level(recovery_ptr->endconePin)) recovery_ptr->endCone = true;
    else recovery_ptr->endCone = false;

    if(gpio_get_level(recovery_ptr->easyIgniterContPin)) recovery_ptr->easyIngiterCont = true;
    else recovery_ptr->easyIngiterCont = false;

    if(gpio_get_level(recovery_ptr->teleIngiterContPin)) recovery_ptr->teleIngiterCont = true;
    else recovery_ptr->teleIngiterCont = false;

}
