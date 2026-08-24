#include "app_state.h"

#define DATA_TIMER_PERIOD_US 20000

static const char * TAG = "APP STATE";

app_state_t app_state;

static esp_timer_handle_t data_timer_handle;

static void on_data_timer(void *arg) {
    app_state.easymini_data = easymini.data;
    app_state.telemetrum_data = telemetrum.data;
    app_state.recovery_data = recovery.data;

    i2c_write(&i2c, (const uint8_t*)&app_state, sizeof(app_state));

    logger_write(&logger_task, &app_state, sizeof(app_state));
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
