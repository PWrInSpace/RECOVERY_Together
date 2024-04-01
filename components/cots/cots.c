#include "cots.h"
#include "esp_log.h"
static const char *TAG = "COTS";

uint8_t cots_init(cots_device_t cots_device){
   
    ESP_LOGI(TAG,"Cots initialization");
 
    if(cots_device == COTS_DEVICE_TELEMETRUM){
        ESP_LOGI(TAG,"**** TELEMETRUM ****");
        gpio_config_t arming_output = {
            .pin_bit_mask = (1ULL << TELE_ARMING),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };

        gpio_config_t igniter_cont_input = {
            .pin_bit_mask = (1ULL << TELE_IGNITER_CONT),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };

        gpio_config_t apogee_input = {
            .pin_bit_mask = (1ULL << TELE_APOGEE_CHECK),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_LOW_LEVEL,
        };

        ESP_ERROR_CHECK(gpio_config(&arming_output));
        ESP_ERROR_CHECK(gpio_config(&igniter_cont_input));
        ESP_ERROR_CHECK(gpio_config(&apogee_input));

        telemetrum_device.armingPin = TELE_ARMING;
        telemetrum_device.igniterPin = TELE_IGNITER_CONT;
        telemetrum_device.apogeePin = TELE_APOGEE_CHECK;

        ESP_LOGI(TAG,"Telemetrum initialization done :D");

    }
    else if(cots_device == COTS_DEVICE_EASYMINI){
        ESP_LOGI(TAG,"***** EASYMINI *****");

        gpio_config_t arming_output = {
            .pin_bit_mask = (1ULL << EASY_ARMING),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };

        gpio_config_t igniter_cont_input = {
            .pin_bit_mask = (1ULL << EASY_IGNITER_CONT),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };

        gpio_config_t apogee_input = {
            .pin_bit_mask = (1ULL << EASY_APOGEE_CHECK),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_LOW_LEVEL,
        };

        ESP_ERROR_CHECK(gpio_config(&arming_output));
        ESP_ERROR_CHECK(gpio_config(&igniter_cont_input));
        ESP_ERROR_CHECK(gpio_config(&apogee_input));

        easymini_device.armingPin = EASY_ARMING;
        easymini_device.igniterPin = EASY_IGNITER_CONT;
        easymini_device.apogeePin = EASY_APOGEE_CHECK;

        ESP_LOGI(TAG,"EasyMini initialization done :D");

    }
    else{
        ESP_LOGE(TAG,"WRONG COTS DEVICE !!!!");
        return RET_FAILTURE;
    }

    return RET_SUCCESS;
}
ESP_LOGE(TAG,"WRONG COTS DEVICE !!!!");

uint8_t cots_arming(cots_device_t cots_device){

    ESP_LOGI(TAG,"COTS ARMING");

    if(cots_device == COTS_DEVICE_TELEMETRUM){

        ESP_LOGI(TAG,"**** TELEMETRUM ****");
        
        if(gpio_set_level(telemetrum_device.armingPin,1) != ESP_OK){
            ESP_LOGE(TAG,"Faild to arm Telemetrum !!!!");
            return RET_FAILTURE;
        }
        telemetrum_device.armStatus = ARMED;

        ESP_LOGI(TAG,"Telemetrum arming done");

    }
    else if(cots_device == COTS_DEVICE_EASYMINI){

        ESP_LOGI(TAG,"***** EASYMINI *****");
        
        if(gpio_set_level(easymini_device.armingPin,1) != ESP_OK){
            ESP_LOGE(TAG,"Faild to arm EasyMini !!!!");
            return RET_FAILTURE;
        }
        easymini_device.armStatus = ARMED;

        ESP_LOGI(TAG,"EasyMini arming done");
    }
    else{
        ESP_LOGE(TAG,"WRONG COTS DEVICE !!!!");
        return RET_FAILTURE;
    }
    return RET_SUCCESS;
      
}

uint8_t cots_disarm(cots_device_t cots_device){

    ESP_LOGI(TAG,"COTS DISARMING");

    if(cots_device == COTS_DEVICE_TELEMETRUM){

        ESP_LOGI(TAG,"**** TELEMETRUM ****");
        
        if(gpio_set_level(telemetrum_device.armingPin,0) != ESP_OK){
            ESP_LOGE(TAG,"Faild to disarm Telemetrum !!!!");
            return RET_FAILTURE;
        }
        telemetrum_device.armStatus = ARMED;

        ESP_LOGI(TAG,"Telemetrum disarming done");

    }
    else if(cots_device == COTS_DEVICE_EASYMINI){

        ESP_LOGI(TAG,"***** EASYMINI *****");
        
        if(gpio_set_level(easymini_device.armingPin,0) != ESP_OK){
            ESP_LOGE(TAG,"Faild to disarm EasyMini !!!!");
            return RET_FAILTURE;
        }
        easymini_device.armStatus = ARMED;

        ESP_LOGI(TAG,"EasyMini disarming done");
    }
    else{
        ESP_LOGE(TAG,"WRONG COTS DEVICE !!!!");
        return RET_FAILTURE;
    }
    return RET_SUCCESS;
}

uint8_t apogee_check(cots_device_t cots_device){

    // TO DO IF NEEDED
    return RET_SUCCESS;

}

