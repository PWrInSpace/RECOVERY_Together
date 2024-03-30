#include "cots.h"

static const char *TAG = "COTS";


uint8_t cots_init(Cots_t *telemetrum_ptr, Cots_t *easymini_ptr){
   
    ESP_LOGI(TAG,"Cots initialization");
 
    gpio_reset_pin(EASY_ARMING);
    gpio_set_direction(EASY_ARMING,GPIO_MODE_OUTPUT);
    gpio_reset_pin(EASY_APOGEE);
    gpio_set_direction(EASY_APOGEE_CHECK,GPIO_MODE_INPUT);
    gpio_reset_pin(EASY_IGNITER_CONT);
    gpio_set_direction(EASY_IGNITER_CONT,GPIO_MODE_INPUT);

    gpio_reset_pin(TELE_ARMING);
    gpio_set_direction(TELE_ARMING,GPIO_MODE_OUTPUT);
    gpio_reset_pin(TELE_APOGEE);
    gpio_set_direction(TELE_APOGEE_CHECK,GPIO_MODE_INPUT);
    gpio_reset_pin(TELE_IGNITER_CONT);
    gpio_set_direction(TELE_IGNITER_CONT,GPIO_MODE_INPUT);


    easymini_ptr->armingPin = EASY_ARMING;
    easymini_ptr->apogeePin = EASY_APOGEE_CHECK;
    easymini_ptr->igniterPin = EASY_IGNITER_CONT;
    telemetrum_ptr->armingPin = TELE_ARMING;
    telemetrum_ptr->apogeePin = TELE_APOGEE_CHECK;
    telemetrum_ptr->igniterPin = TELE_IGNITER_CONT;

    return RET_SUCCESS
}


uint8_t cots_arming(Cots_t *telemetrum_ptr, Cots_t *easymini_ptr){
    gpio_set_level(telemetrum_ptr->armingPin,1);
    telemetrum_ptr->armStatus = true;
    gpio_set_level(easymini_ptr->armingPin,1);
    easymini_ptr->armStatus = true;
    if(telemetrum_ptr->armStatus & easymini_ptr->armStatus)
        return RET_SUCCESS

    return RET_FEILTURE    
}

uint8_t cots_disarm(Cots_t *telemetrum_ptr, Cots_t *easymini_ptr){
    gpio_set_level(telemetrum_ptr->armingPin,0);
    telemetrum_ptr->armStatus = false;
    gpio_set_level(easymini_ptr->armingPin,0);
    easymini_ptr->armStatus = false;

    if(telemetrum_ptr->armStatus & easymini_ptr->armStatus)
        return RET_FEILTURE

    return RET_SUCCESS;
}

uint8_t apogee_check(Cots_t *telemetrum_ptr, Cots_t *easymini_ptr){

    if(gpio_get_level(telemetrum_ptr->apogeePin)){
        // pilot parachute deploy !!!
        telemetrum_ptr->apogeeDetection = true; 
    }

    if(gpio_get_level(easymini_ptr->apogeePin)){
        // pilot parachute deploy !!!
        easymini_ptr->apogeeDetection = true;
    }

    if(easymini_ptr->apogeeDetection | telemetrum_ptr->apogeeDetection)
        return APOGEE;

    return OTHER_STATE;

}

