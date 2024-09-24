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
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"

static const char *TAG = "APP";

recovery_data_t data_to_send;
Pressure_Sensor_t pressure_sensor;
adc_cali_handle_t handle = NULL;
adc_oneshot_unit_handle_t adc_handle = NULL;

void read_data(){

    data_to_send.isArmed = easymini_device.armStatus; //  EasyMini arm status
    data_to_send.isTeleActive =  telemetrum_device.armStatus; // Telemetrum arm status
    data_to_send.easyMiniFirstStage = easymini_device.apogeeDetection; // easyminiApogee Detection
    data_to_send.easyMiniSecondStage = recovery_system.easySecondStage; // easyMini igniter fire
    data_to_send.telemetrumFirstStage = telemetrum_device.apogeeDetection; // Telemetrum apogee detection
    data_to_send.telemetrumSecondStage = recovery_system.teleSecondStage; // telemetrum igniter fire
    data_to_send.firstStageDone = recovery_system.firstStageDone; // first stage confirmation
    data_to_send.secondStageDone = recovery_system.secondStageDone; // second stage confirmation
    data_to_send.firstStageContinouity = !gpio_get_level(recovery_system.easyIgniterContPin); // easyIgniterCont
    data_to_send.secondStageContinouity = !gpio_get_level(recovery_system.teleIgniterContPin); // TeleIgniterCont
    data_to_send.separationSwitch1 = !gpio_get_level(END_CONE);
    data_to_send.pressure1 = (uint16_t)get_pressure(&pressure_sensor);
    ESP_LOGI(TAG,"PRESSURE: %d", data_to_send.pressure1);

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



bool adc_init(){
    esp_err_t ret = ESP_FAIL;
    esp_err_t res;
    bool calibrated = false;

    pressure_sensor.adc_cali_handle = &handle;
  if (ret == ESP_OK) {
    ESP_LOGI(TAG, "Calibration Success");
  } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
    ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
  } else {
    ESP_LOGE(TAG, "Invalid arg or no memory");
  }

    adc_oneshot_unit_init_cfg_t adc_init_config_2 = {
      .unit_id = ADC_UNIT,
  };
  ESP_ERROR_CHECK(
      adc_oneshot_new_unit(&adc_init_config_2, &adc_handle));
    pressure_sensor.adc_handle = &adc_handle;

    if (!calibrated) {
    ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT,
        .atten = ADC_ATTEN_PRESSURE,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
    if (ret == ESP_OK) {
      calibrated = true;
    }
  }

    pressure_sensor.cali_enable = calibrated;
    pressure_sensor.adc_channel = ADC_CHANNEL;
    res = pressure_sensor_init(&pressure_sensor);
    return res;
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

    adc_init();

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
    }
}
