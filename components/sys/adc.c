#include "adc.h"

const static char *TAG = "ADC";

esp_err_t adc_init(const adc_config_t *config, adc_t *adc) {
    *adc = (adc_t){
        .config = *config,
        .handle = NULL,
    };

    const adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = config->unit_id,
    };
    if (adc_oneshot_new_unit(&unit_cfg, &adc->handle) != ESP_OK) {
        ESP_LOGE(TAG, "unable to create new ADC unit");
        return ESP_FAIL;
    }

    const adc_oneshot_chan_cfg_t chan_cfg = {
        .bitwidth = config->bitwidth,
        .atten = config->attenuation,
    };
    if (adc_oneshot_config_channel(adc->handle, config->channel, &chan_cfg) != ESP_OK) {
        ESP_LOGE(TAG, "unable to configure ADC channel");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t adc_deinit(const adc_t *adc) {
    return adc_oneshot_del_unit(adc->handle);
}

esp_err_t adc_read(const adc_t *adc, int *value) {
    if (adc_oneshot_read(adc->handle, adc->config.channel, value) != ESP_OK) {
        ESP_LOGE(TAG, "unable to read ADC value");
        return ESP_FAIL;
    }
    return ESP_OK;
}