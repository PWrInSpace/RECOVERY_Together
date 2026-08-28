#include "app_state.h"

#define DATA_TIMER_PERIOD_US 20000
#define WRITE_TO_MCB_TIMER_PERIOD_US 500000

static const char * TAG = "APP STATE";

app_state_t app_state;

static esp_timer_handle_t data_timer_handle;

static esp_timer_handle_t write_to_mcb_timer_handle;

static void on_data_timer(void *arg) {
    app_state.easymini_data = easymini.data;
    app_state.telemetrum_data = telemetrum.data;
    app_state.recovery_data = recovery.data;

    logger_write(&logger_task, &app_state, sizeof(app_state));
}

static void on_write_to_mcb(void *arg) {
    const i2c_data_t i2c_data = {
        .telemetrum_armed = app_state.telemetrum_data.armed,
        .telemetrum_apogee_detected = app_state.telemetrum_data.apogee_detected,
        .telemetrum_first_stage = app_state.telemetrum_data.first_stage,
        .telemetrum_second_stage = app_state.telemetrum_data.second_stage,
        .easymini_armed = app_state.easymini_data.armed,
        .easymini_apogee_detected = app_state.easymini_data.apogee_detected,
        .easymini_first_stage = app_state.easymini_data.first_stage,
        .easymini_second_stage = app_state.easymini_data.second_stage,
        .separation_one = app_state.recovery_data.separation_one,
        .separation_two = app_state.recovery_data.separation_two,
        .continuity = app_state.continuity
    };

    i2c_write(&i2c, (const uint8_t*)&i2c_data, sizeof(i2c_data));
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

    esp_timer_create_args_t timer_arg = {
        .arg = NULL,
        .callback = on_data_timer,
        .name = "data timer",
    };
    esp_timer_create(&timer_arg, &data_timer_handle);
    esp_timer_start_periodic(data_timer_handle, DATA_TIMER_PERIOD_US);

    timer_arg.callback = on_write_to_mcb;
    timer_arg.name = "write to mcb timer";
    esp_timer_create(&timer_arg, &write_to_mcb_timer_handle);
    esp_timer_start_periodic(write_to_mcb_timer_handle, WRITE_TO_MCB_TIMER_PERIOD_US);

    return ESP_OK;
}
