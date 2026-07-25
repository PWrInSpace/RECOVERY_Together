#include "cots.h"
#include "esp_log.h"

static const char *TAG = "COTS";

esp_err_t cots_init(cots_t *cots){
    ESP_LOGI(TAG,"Cots initialization");
 
    gpio_config_t arming_output = {
        .pin_bit_mask = (1ULL << cots->arming_pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config_t apogee_input = {
        .pin_bit_mask = (1ULL << cots->apogee_pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    gpio_config_t main_input = {
        .pin_bit_mask = (1ULL << cots->main_pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    ESP_ERROR_CHECK(gpio_config(&arming_output));
    ESP_ERROR_CHECK(gpio_config(&apogee_input));
    ESP_ERROR_CHECK(gpio_config(&main_input));

    cots_disarm(cots);

    ESP_LOGI(TAG, "Cots initialized");

    return ESP_OK;
}


esp_err_t cots_arming(cots_t *cots){
    ESP_LOGI(TAG, "Cots arming");

    if (gpio_set_level(cots->arming_pin, 1) != ESP_OK) {
        ESP_LOGE(TAG,"Failed to arm");
        return ESP_FAIL;
    }
    cots->armed = ARMED;

    ESP_LOGI(TAG, "Cots arming done");

    return ESP_OK;
}

esp_err_t cots_disarm(cots_t *cots){
    ESP_LOGI(TAG,"Cots disarming");

    if (gpio_set_level(cots->arming_pin, 0) != ESP_OK) {
        ESP_LOGE(TAG,"Failed to disarm");
        return ESP_FAIL;
    }
    cots->armed = DISARMED;

    ESP_LOGI(TAG,"Cots disarming done");

    return ESP_OK;
}

esp_err_t apogee_check(cots_t *cots){
    if (gpio_get_level(cots->apogee_pin)) {
        cots->apogee_detected = true;
    } else {
        cots->apogee_detected = false;
    }

    return ESP_OK;
}

