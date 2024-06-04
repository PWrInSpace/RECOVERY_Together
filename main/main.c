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

void read_data(){

    data_to_send.isArmed = easymini_device.armStatus;
    data_to_send.isTeleActive =  telemetrum_device.armStatus;
    data_to_send.easyMiniFirstStage = easymini_device.apogeeDetection;
    data_to_send.easyMiniSecondStage = recovery_system.easySecondStage;
    data_to_send.telemetrumFirstStage = telemetrum_device.apogeeDetection;
    data_to_send.telemetrumSecondStage = recovery_system.teleSecondStage;
    data_to_send.firstStageDone = recovery_system.firstStageDone;
    data_to_send.secondStageDone = recovery_system.secondStageDone;
    data_to_send.firstStageContinouity = recovery_system.easyIgniterCont;
    data_to_send.secondStageContinouity = recovery_system.teleIgniterCont;
    data_to_send.separationSwitch1 = !gpio_get_level(END_CONE);

}

void execute_cmd(uint32_t data){

    switch (data)
    {
    case PILOT_DEPLOY_CMD:

        gpio_set_level(LED,1);
        rx_buffer.cmd.command = 0;
        rx_buffer.cmd.payload = 0;
        break;

    case MAIN_DEPLOY_CMD:

        gpio_set_level(LED,0);
        rx_buffer.cmd.command = 0;
        rx_buffer.cmd.payload = 0;
        break;

    case TELEMETRUM_ARM_CMD:

        gpio_set_level(LED,1);
        rx_buffer.cmd.command = 0;
        rx_buffer.cmd.payload = 0;
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

void init_adc(adc_cali_handle_t *handle){
    ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_2,
        .atten = ADC_ATTEN_PRESSURE,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_cali_create_scheme_line_fitting(&cali_config, handle);
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

    adc_cali_handle_t handle;
     adc_oneshot_unit_handle_t handle_oneshot;
    init_adc(&handle);
    adc_oneshot_unit_init_cfg_t adc_init_config_1 = {
      .unit_id = ADC_UNIT_2,
     };
     ESP_ERROR_CHECK(
      adc_oneshot_new_unit(&adc_init_config_1, &handle_oneshot));
    pressure_sensor = (Pressure_Sensor_t)PRESSURE_SENSOR_INIT(PRESSURE,&handle_oneshot,&handle,true);
    pressure_sensor_init(&pressure_sensor);
    
    /************************ INTERRUPT HANDLERS *****************************/
    gpio_install_isr_service(0);
    gpio_isr_handler_add(TELE_APOGEE_CHECK,tele_apogee_isr_handler,NULL);
    gpio_isr_handler_add(EASY_APOGEE_CHECK,easy_apogee_isr_handler,NULL);

    ESP_LOGI(TAG,"Init completed, entering recovery loop");

    cots_arming(COTS_DEVICE_TELEMETRUM);
    ESP_LOGI(TAG, "Status: %d",telemetrum_device.armStatus);
    cots_arming(COTS_DEVICE_EASYMINI);
    //ESP_LOGI(TAG, "Status: %d",telemetrum_device.armStatus);
    read_data();
    while(1){
       data_to_send.pressure1 = (uint16_t)get_pressure(&pressure_sensor);
        vTaskDelay(1000/portTICK_PERIOD_MS);
        read_data();
        I2C_buffer_write();
    }
}
