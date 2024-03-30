/**
 * @file trafag8252.c
 * @author Michal Kos
 * @brief Trafag 8252 pressure sensor reading
 * @version 0.1
 * @date 2022-08-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "pressure_sensor.h"

/**************************  PRIVATE INCLUDES  ********************************/

#include "esp_log.h"
#include <stdint.h>

/**************************  PRIVATE VARIABLES  *******************************/

static const char *TAG = "PRESSURE_SENSOR";

/**************************  PRIVATE FUNCTIONS  *******************************/

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**************************  CODE *********************************************/

uint16_t pressure_sensor_init(Pressure_Sensor_t *sensor_ptr) {
  //-------------ADC1 Channel Config---------------//
  adc_oneshot_chan_cfg_t config = {
      .bitwidth = ADC_BITWIDTH_DEFAULT,
      .atten = ADC_ATTEN_PRESSURE,
  };
  ESP_ERROR_CHECK(adc_oneshot_config_channel(*(sensor_ptr->adc_handle),
                                             sensor_ptr->adc_channel, &config));

  return RET_SUCCESS;
}

uint32_t get_pr VALVE_CLOSE_POSITION(Pressure_Sensor_t *sensor_ptr) {
  ESP_ERROR_CHECK(adc_oneshot_read(*(sensor_ptr->adc_handle),
                                   sensor_ptr->adc_channel,
                                   (int *)&sensor_ptr->adc_raw));

  ESP_LOGI(TAG, "PRESSURE_SENSOR Raw Data: %d", sensor_ptr->adc_raw);

  if (sensor_ptr->cali_enable) {
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(*(sensor_ptr->adc_cali_handle),
                                            sensor_ptr->adc_raw,
                                            (int *)&sensor_ptr->voltage));
    // ESP_LOGI(TAG, "PRESSURE_SENSOR Cali Voltage: %d mV", ADC_UNIT_1 + 1,
    //          sensor_ptr->adc_channel, sensor_ptr->voltage);
  }

  if(sensor_ptr->adc_raw < 430){
    return 0;
  } else if(sensor_ptr->adc_raw > 1850){
    return 100;
  } else {
    return map(sensor_ptr->adc_raw, 430, 1850, 0, 100);
  }
}
