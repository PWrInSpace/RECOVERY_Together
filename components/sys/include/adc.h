#ifndef ADC_H
#define ADC_H

#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

typedef struct {
    uint8_t unit_id;
    uint8_t attenuation;
    uint8_t bitwidth;
    uint8_t channel;
    uint8_t v_max; // maximum voltage in mV for the ADC channel
} adc_config_t;

typedef struct {
    adc_config_t config;
    adc_oneshot_unit_handle_t handle;
} adc_t;

esp_err_t adc_init(const adc_config_t *config, adc_t *adc);

esp_err_t adc_deinit(const adc_t *adc);

esp_err_t adc_read_raw(const adc_t *adc, int *value);

esp_err_t adc_read(const adc_t *adc, int *value);

#endif
