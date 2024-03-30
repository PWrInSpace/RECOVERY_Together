/**
 * @file pressure_sensor.h
 * @author Michal Kos
 * @brief Trafag 8252 pressure sensor reading
 * @version 0.1
 * @date 2022-08-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PRESSURE_H
#define PRESSURE_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdint.h>

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"

/**************************  CONFIG INCLUDES  *********************************/

#include "config.h"
#include "pinout.h"

/**************************  DEFINES  *****************************************/

// ADC channel attenuation - can be changed
#define ADC_ATTEN_PRESSURE ADC_ATTEN_DB_11

/**************************  INIT *********************************************/

#define PRESSURE_SENSOR_INIT(X, Y, Z, Q)                            \
  {                                                                 \
    .adc_channel = X, .cali_enable = Q, .adc_raw = 0, .voltage = 0, \
    .adc_handle = Y, .adc_cali_handle = Z                           \
  }

/**************************  PUBLIC VARIABLES  ********************************/

/**************************  CODE *********************************************/

typedef struct {
    adc_channel_t adc_channel;
    uint32_t adc_raw;
    uint32_t voltage;
    adc_oneshot_unit_handle_t *adc_handle;
    adc_cali_handle_t *adc_cali_handle;
    bool cali_enable;
} Pressure_Sensor_t;

uint16_t pressure_sensor_init(Pressure_Sensor_t *sensor_ptr);

uint32_t get_pressure(Pressure_Sensor_t *sensor_ptr);

#endif  // PRESSURE_H
