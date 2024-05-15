#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"

#include "config.h"
#include "pinout.h"
#include "cots.h"
#include "dataStructs.h"
#include "Pressure_sensor.h"
#include "recovery_control.h"
#include "i2c.h"

static const char *TAG = "APP";

data_to_mcb data_to_send;

void read_data(){

    data_to_send.isArmed = easymini_device.armStatus;
    data_to_send.isTeleActive =  telemetrum_device.armStatus;
    data_to_send.easyMiniFirstStage = easymini_device.apogeeDetection;
    data_to_send.easyMiniSecondStage = recovery_system.easySecondStage;
    data_to_send.telemetrumFirstStage = telemetrum_device.apogeeDetection;
    data_to_send.telemetrumSecondStage = recovery_system.teleSecondStage;
    data_to_send.firstStageDone = recovery_system.firstStageDone;
    data_to_send.secondStageDone = recovery_system.secondStageDone;
    data_to_send.firstStageContinouity = recovery_system.endCone;
   // data_to_send.pressure = ;

}

void execute_cmd(uint32_t data[2]){

    uint32_t cmd = data[0];
    switch (cmd)
    {
    case PILOT_DEPLOY_CMD:

        first_Stage_Deploy();
        break;

    case MAIN_DEPLOY_CMD:

        second_Stage_Deploy();
        break;

    case TELEMETRUM_ARM_CMD:

        cots_arming(COTS_DEVICE_TELEMETRUM);
        break;
    
    case TELEMETRUM_DISARM_CMD:

        cots_disarm(COTS_DEVICE_TELEMETRUM);
        break;

    case EASYMINI_ARM_CMD:

        cots_arming(COTS_DEVICE_EASYMINI);
        break;

    case EASYMINI_DISARM_CMD:

        cots_disarm(COTS_DEVICE_EASYMINI);
        break;

    default:
        break;
    }         
}


void app_main(void)
{

    /***************************** APP INITIALIZATION *************************/
    if(cots_init(COTS_DEVICE_TELEMETRUM) != RET_SUCCESS){
        ESP_LOGE(TAG,"Telemetrum init failed, restarting ...");
        esp_restart();
    }

    if(cots_init(COTS_DEVICE_EASYMINI) != RET_SUCCESS){
        ESP_LOGE(TAG,"EasyMini init failed, restarting ...");
        esp_restart();
    }

    if(recovery_Init() != RET_SUCCESS){
        ESP_LOGE(TAG,"Recovery init failed, restarting ...");
        esp_restart();
    }


    if(I2C_slave_init() != RET_SUCCESS){
        ESP_LOGE(TAG,"I2C slave init failed, restarting ...");
        esp_restart();
    }

    /************************ INTERRUPT HANDLERS *****************************/
    gpio_isr_register(&apogee_isr_handler,NULL, ESP_INTR_FLAG_LEVEL3,NULL);
    
    ESP_LOGI(TAG,"Init completed, entering recovery loop");

    while(1){

        apogee_check();
        read_data();
        //I2C_buffer_write();
        //I2C_buffer_read();   
        execute_cmd(rx_buffer);
        ESP_LOGI(TAG, "Status: %d",easymini_device.armStatus);
        cots_arming(COTS_DEVICE_EASYMINI);
        vTaskDelay(1000/ portTICK_PERIOD_MS);
        read_data();
        ESP_LOGI(TAG, "Status: %d",easymini_device.armStatus);
        cots_disarm(COTS_DEVICE_EASYMINI);
        

    }
}
