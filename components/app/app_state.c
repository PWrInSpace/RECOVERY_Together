#include "app_state.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define DATA_TIMER_PERIOD_US 20000
#define WRITE_TO_MCB_TIMER_PERIOD_US 500000

static const char *TAG = "APP STATE";

app_state_t app_state;

static SemaphoreHandle_t app_state_mutex;
static StaticSemaphore_t app_state_mutex_buffer;

static esp_timer_handle_t data_timer_handle;

static esp_timer_handle_t write_to_mcb_timer_handle;

static void on_data_timer(void *arg) {
    cots_data_t easy_data, tele_data;
    recovery_data_t rec_data;

    cots_get_data(&easymini, &easy_data);
    cots_get_data(&telemetrum, &tele_data);
    recovery_get_data(&recovery, &rec_data);

    xSemaphoreTake(app_state_mutex, portMAX_DELAY);
    app_state.easymini_data = easy_data;
    app_state.telemetrum_data = tele_data;
    app_state.recovery_data = rec_data;
    
    app_state_t state_copy = app_state;
    xSemaphoreGive(app_state_mutex);

    logger_write(&logger_task, &state_copy, sizeof(app_state));
}

static void on_write_to_mcb(void *arg) {
    xSemaphoreTake(app_state_mutex, portMAX_DELAY);
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
        .continuity = app_state.continuity,
        .first_stage = app_state.recovery_data.first_stage,
        .second_stage = app_state.recovery_data.second_stage,
    };
    xSemaphoreGive(app_state_mutex);

    i2c_write(&i2c, (const uint8_t*)&i2c_data, sizeof(i2c_data));
}

esp_err_t init_app_state(void) {
    app_state_mutex = xSemaphoreCreateMutexStatic(&app_state_mutex_buffer);

    app_state = (app_state_t){
        .easymini_data = {0},
        .telemetrum_data = {0},
        .recovery_data = {0},
    };

    cots_get_data(&easymini, &app_state.easymini_data);
    cots_get_data(&telemetrum, &app_state.telemetrum_data);
    recovery_get_data(&recovery, &app_state.recovery_data);

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
