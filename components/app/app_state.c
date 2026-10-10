#include "app_state.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define DATA_TIMER_PERIOD_US 20000
#define WRITE_TO_MCB_TIMER_PERIOD_US 500000
#define RESISTANCE_TIMER 500000
#define RESISTANCE_WIRE_CONT_THRESHOLD 2000

static const char *TAG = "APP STATE";

app_state_t app_state;

static StaticSemaphore_t app_state_mutex_buffer;
static esp_timer_handle_t data_timer_handle;
static esp_timer_handle_t write_to_mcb_timer_handle;
static esp_timer_handle_t continuity_timer;
static adc_t adc;

static void on_data_timer(void *arg) {
    cots_data_t easy_data, tele_data;
    recovery_data_t rec_data;

    cots_get_data(&bluejay, &easy_data);
    cots_get_data(&telemetrum, &tele_data);
    recovery_get_data(&recovery, &rec_data);

    xSemaphoreTake(app_state.mutex, pdMS_TO_TICKS(100));
    app_state.bluejay_data = easy_data;
    app_state.telemetrum_data = tele_data;
    app_state.recovery_data = rec_data;

    const app_state_t state_copy = app_state;
    xSemaphoreGive(app_state.mutex);

    logger_write(&logger_task, &state_copy, sizeof(app_state));
}

static esp_err_t setup_data_timer() {
    esp_timer_create_args_t timer_arg = {
        .callback = on_data_timer,
        .name = "data timer",
    };

    if (esp_timer_create(&timer_arg, &data_timer_handle) != ESP_OK) {
        return ESP_FAIL;
    }

    if (esp_timer_start_periodic(data_timer_handle, DATA_TIMER_PERIOD_US) != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

static void on_write_to_mcb(void *arg) {
    xSemaphoreTake(app_state.mutex, pdMS_TO_TICKS(100));
    const i2c_data_t i2c_data = {
        .telemetrum_armed = app_state.telemetrum_data.armed,
        .telemetrum_apogee_detected = app_state.telemetrum_data.apogee_detected,
        .telemetrum_main_deployed = app_state.telemetrum_data.main_deployed,
        .bluejay_armed = app_state.bluejay_data.armed,
        .bluejay_apogee_detected = app_state.bluejay_data.apogee_detected,
        .bluejay_main_deployed = app_state.bluejay_data.main_deployed,
        .separation_one = app_state.recovery_data.separation_one,
        .separation_two = app_state.recovery_data.separation_two,
        .continuity = app_state.continuity,
        .wire_on = app_state.wire_on,
        .recovery_first_stage = app_state.recovery_data.first_stage,
        .recovery_second_stage = app_state.recovery_data.second_stage,
    };
    xSemaphoreGive(app_state.mutex);

    i2c_write(&i2c, (const uint8_t*)&i2c_data, sizeof(i2c_data));
}

static esp_err_t setup_write_to_mcb_timer() {
    esp_timer_create_args_t timer_arg = {
        .callback = on_write_to_mcb,
        .name = "write to mcb timer",
    };

    if (esp_timer_create(&timer_arg, &write_to_mcb_timer_handle) != ESP_OK) {
        return ESP_FAIL;
    }

    if (esp_timer_start_periodic(write_to_mcb_timer_handle, WRITE_TO_MCB_TIMER_PERIOD_US) != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

static adc_config_t adc_config = {
    .unit_id = ADC_UNIT_1,
    .bitwidth = ADC_BITWIDTH_12,
    .attenuation = ADC_ATTEN_DB_12,
    .channel = ADC_CHANNEL_0
};

static void resistance_wire_continuity(void *arg) {
    int raw_value;
    bool res;
    if (adc_read_raw(&adc, &raw_value) != ESP_OK) {
        res = false;
    } else {
        res = raw_value > RESISTANCE_WIRE_CONT_THRESHOLD;
    }

    xSemaphoreTake(app_state.mutex, pdMS_TO_TICKS(10));
    app_state.continuity = res;
    xSemaphoreGive(app_state.mutex);
}

static esp_err_t setup_continuity_timer() {
    const esp_timer_create_args_t timer_args = {
        .callback = resistance_wire_continuity,
        .name = "resistance_wire_continuity_timer"
    };

    if (esp_timer_create(&timer_args, &continuity_timer) != ESP_OK) {
        return ESP_FAIL;
    }

    if (esp_timer_start_periodic(continuity_timer, RESISTANCE_TIMER) != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t init_app_state(void) {
    app_state = (app_state_t){
        .bluejay_data = {0},
        .telemetrum_data = {0},
        .recovery_data = {0},
        .continuity = false,
        .wire_on = false,
        .mutex = xSemaphoreCreateMutexStatic(&app_state_mutex_buffer)
    };

    if (app_state.mutex == NULL) {
        return ESP_FAIL;
    }

    cots_get_data(&bluejay, &app_state.bluejay_data);
    cots_get_data(&telemetrum, &app_state.telemetrum_data);
    recovery_get_data(&recovery, &app_state.recovery_data);

    if (setup_data_timer() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to setup data timer");
        return ESP_FAIL;
    }

    if (setup_write_to_mcb_timer() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to setup write to mcb timer");
        return ESP_FAIL;
    }

    if (adc_init(&adc_config, &adc) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize ADC");
        return ESP_FAIL;
    }

    if (setup_continuity_timer() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to setup continuity timer");
        return ESP_FAIL;
    }

    return ESP_OK;
}
