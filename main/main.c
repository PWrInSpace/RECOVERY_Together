#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"

#include "config.h"
#include "pinout.h"
#include "cots.h"
#include "dataStructs.h"
#include "Pressure_sensor.h"
#include "recovery_control.h"

static const char *TAG = "APP";


void app_main(void)
{

    /***************************** APP INITIALIZATION *************************/
    if(recovery_Init() != RET_SUCCESS){
        ESP_LOGE(TAG,"Recovery init failed, restarting ...");
        esp_restart();
    }

    if(cots_init(COTS_DEVICE_TELEMETRUM) != RET_SUCCESS){
        ESP_LOGE(TAG,"Telemetrum init failed, restarting ...");
        esp_restart();
    }

    if(cots_init(COTS_DEVICE_EASYMINI) != RET_SUCCESS){
        ESP_LOGE(TAG,"EasyMini init failed, restarting ...");
        esp_restart();
    }

    /************************ INTERRUPT HANDLERS *****************************/
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_NMI));
    ESP_ERROR_CHECK(gpio_isr_handler_add(TELE_APOGEE_CHECK, apogee_isr_handler,NULL));
    ESP_ERROR_CHECK(gpio_isr_handler_add(EASY_APOGEE_CHECK, apogee_isr_handler,NULL));
    
    ESP_LOGI(TAG,"Init completed, entering recovery loop");

    while(1){

        check_Cont();

        // ADD PRESSURE MEASURE
        // ADD I2C CMDs HANDLERS AND DATA SENDING 

        

    }
}
