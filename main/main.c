#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"

#include "config.h"
#include "pinout.h"
#include "cots.h"
#include "dataStructs.h"
#include "recovery_control.h"
#include "i2c.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "console_config.h"

static const char *TAG = "APP";

recovery_data_t data_to_send;

void read_data(){

    data_to_send.isArmed = easymini_device.armStatus; //  EasyMini arm status
    data_to_send.isTeleActive =  telemetrum_device.armStatus; // Telemetrum arm status
    data_to_send.easyMiniFirstStage = easymini_device.apogeeDetection; // easyminiApogee Detection
    data_to_send.easyMiniSecondStage = recovery_system.easySecondStage; // easyMini igniter fire
    data_to_send.telemetrumFirstStage = telemetrum_device.apogeeDetection; // Telemetrum apogee detection
    data_to_send.telemetrumSecondStage = recovery_system.teleSecondStage; // telemetrum igniter fire
    data_to_send.firstStageDone = recovery_system.firstStageDone; // first stage confirmation
    data_to_send.secondStageDone = recovery_system.secondStageDone; // second stage confirmation
    data_to_send.separationSwitch1 = !gpio_get_level(END_CONE_1);
    data_to_send.separationSwitch2 = !gpio_get_level(END_CONE_2);
    //ESP_LOGI(TAG,"PRESSURE: %d", data_to_send.pressure1);

}

void execute_cmd(uint32_t data){

    switch (data)
    {
    case EASYMINI_ARM_CMD:

        cots_arming(COTS_DEVICE_EASYMINI);
        rx_buffer.cmd.command = 0x00;
        break;

    case EASYMINI_DISARM_CMD:

        cots_disarm(COTS_DEVICE_EASYMINI);
        rx_buffer.cmd.command = 0x00;
        break;

    case TELEMETRUM_ARM_CMD:

        cots_arming(COTS_DEVICE_TELEMETRUM);  
        rx_buffer.cmd.command = 0x00;    
        break;
    
    case TELEMETRUM_DISARM_CMD:

        cots_disarm(COTS_DEVICE_TELEMETRUM);
        rx_buffer.cmd.command = 0x00;
        break;

    case FORCE_FIRST_STAGE_CMD:

        first_Stage_Deploy();
        rx_buffer.cmd.command = 0x00;
        break;

    case FORCE_SECOND_STAGE_CMD:

        second_Stage_Deploy();
        rx_buffer.cmd.command = 0x00;
        break;

    default:
        break;
    }         
}

void log_debug_data(void);

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
     
    servo_init();

    if(init_console() != ESP_OK){
        ESP_LOGE(TAG,"Console init failed, restarting ...");
        esp_restart();
    }

    /************************ INTERRUPT HANDLERS *****************************/
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    gpio_isr_handler_add(TELE_APOGEE_CHECK,tele_apogee_isr_handler,NULL);
    gpio_isr_handler_add(EASY_APOGEE_CHECK,easy_apogee_isr_handler,NULL);

    ESP_LOGI(TAG,"Init completed, entering recovery loop");
    gpio_set_level(LED,1);


    read_data();

    while(1){
        I2C_buffer_read();
        execute_cmd(rx_buffer.cmd.command);
        read_data();
        check_Cont();
        I2C_buffer_write();
        vTaskDelay(pdMS_TO_TICKS(100));
        log_debug_data();
    }
}

void log_debug_data() {
    ESP_LOGI(TAG, "**** RECOVERY DATA ****");
    ESP_LOGI(TAG, "first stage done: %d", recovery_system.firstStageDone);
    ESP_LOGI(TAG, "second stage done: %d", recovery_system.secondStageDone);
    ESP_LOGI(TAG, "easy mini igniter continuity: %d", recovery_system.easyIgniterCont);
    ESP_LOGI(TAG, "telemetrum igniter continuity: %d", recovery_system.teleIgniterCont);
    ESP_LOGI(TAG, "end cone: %d", recovery_system.endCone);
    ESP_LOGI(TAG, "easy mini second stage: %d", recovery_system.easySecondStage);
    ESP_LOGI(TAG, "telemetrum second stage: %d", recovery_system.teleSecondStage);
    ESP_LOGI(TAG, "second stage continuity: %d", recovery_system.secondStageCont);
    ESP_LOGI(TAG, "apogee detection: %d", recovery_system.apogeeDetection);
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "**** BLUE JAY ****");
    ESP_LOGI(TAG, "armed: %d", telemetrum_device.armStatus);
    ESP_LOGI(TAG, "apogee detection: %d", telemetrum_device.apogeeDetection);
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
}
