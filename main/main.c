#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"

#include "config.h"
#include "pinout.h"
#include "../components/cots/include/cots.h"
#include "recovery.h"
#include "../components/i2c/include/i2c.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "console_config.h"

// static const char *TAG = "APP";

recovery_data_t data_to_send;

void read_data(){
}

void execute_cmd(uint32_t data){
    //
    // switch (data)
    // {
    // case EASYMINI_ARM_CMD:
    //
    //     cots_arming(COTS_DEVICE_2);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // case EASYMINI_DISARM_CMD:
    //
    //     cots_disarm(COTS_DEVICE_2);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // case TELEMETRUM_ARM_CMD:
    //
    //     cots_arming(COTS_DEVICE_1);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // case TELEMETRUM_DISARM_CMD:
    //
    //     cots_disarm(COTS_DEVICE_1);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // case FORCE_FIRST_STAGE_CMD:
    //
    //     first_Stage_Deploy(true);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // case FORCE_SECOND_STAGE_CMD:
    //
    //     second_Stage_Deploy(true);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // default:
    //     break;
    // }
}

void app_main(void)
{
    //
    // /***************************** APP INITIALIZATION *************************/
    // if(cots_init(COTS_DEVICE_1) != RET_SUCCESS){
    //     ESP_LOGE(TAG,"Telemetrum init failed, restarting ...");
    //     esp_restart();
    // }
    //
    // if(cots_init(COTS_DEVICE_2) != RET_SUCCESS){
    //     ESP_LOGE(TAG,"EasyMini init failed, restarting ...");
    //     esp_restart();
    // }
    //
    // if(recovery_Init() != RET_SUCCESS){
    //     ESP_LOGE(TAG,"Recovery init failed, restarting ...");
    //     esp_restart();
    // }
    //
    //
    // if(I2C_slave_init() != RET_SUCCESS){
    //     ESP_LOGE(TAG,"I2C slave init failed, restarting ...");
    //     esp_restart();
    //  }
    //
    // servo_init();
    //
    // if(init_console() != ESP_OK){
    //     ESP_LOGE(TAG,"Console init failed, restarting ...");
    //     esp_restart();
    // }
    //
    // /************************ INTERRUPT HANDLERS *****************************/
    // gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    // gpio_isr_handler_add(TELE_APOGEE_CHECK, tele_apogee_isr_handler, NULL);
    // gpio_isr_handler_add(EASY_APOGEE_CHECK, easy_apogee_isr_handler, NULL);
    // gpio_isr_handler_add(TELE_IGNITER_FIRE, tele_main_isr_handler, NULL);
    //
    // ESP_LOGI(TAG,"Init completed, entering recovery loop");
    // gpio_set_level(LED,1);
    //
    //
    // read_data();
    //
    // while (true) {
    //     I2C_buffer_read();
    //     execute_cmd(rx_buffer.cmd.command);
    //     read_data();
    //     check_Cont();
    //     I2C_buffer_write();
    //     vTaskDelay(pdMS_TO_TICKS(100));
    //     log_debug_data();
    // }
}