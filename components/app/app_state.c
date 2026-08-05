#include "app_state.h"

#define DATA_TIMER_PERIOD_US 20000

static const char * TAG = "APP STATE";

static esp_timer_handle_t data_timer_handle;
static esp_timer_handle_t test_timer_handle;

static void on_data_timer(void *arg) {
    app_state.easymini_data = easymini.data;
    app_state.telemetrum_data = telemetrum.data;
    app_state.recovery_data = recovery.data;

    logger_write(&logger_task, &app_state, sizeof(app_state));
}

static void on_test_timer(void *arg) {
    ESP_LOGI(TAG, "writing data");
    const float current_time = (float)esp_timer_get_time() / 1000000.0f;
    logger_write(&logger_task, &current_time, sizeof(float));
}

esp_err_t init_app_state(void) {
    app_state = (app_state_t){
        .easymini_data = {0},
        .telemetrum_data = {0},
        .recovery_data = {0},
    };

    app_state.easymini_data = easymini.data;
    app_state.telemetrum_data = telemetrum.data;
    app_state.recovery_data = recovery.data;

    const esp_timer_create_args_t timer_arg = {
        .arg = NULL,
        .callback = on_data_timer,
        .name = "data timer",
    };

    esp_timer_create(&timer_arg, &data_timer_handle);
    esp_timer_start_periodic(data_timer_handle, DATA_TIMER_PERIOD_US);

    return ESP_OK;
}

esp_err_t init_app_state_test(void) {
    const esp_timer_create_args_t timer_arg = {
        .arg = NULL,
        .callback = on_test_timer,
        .name = "test timer",
    };

    esp_timer_create(&timer_arg, &test_timer_handle);
    esp_timer_start_periodic(test_timer_handle, DATA_TIMER_PERIOD_US);

    return ESP_OK;
}
