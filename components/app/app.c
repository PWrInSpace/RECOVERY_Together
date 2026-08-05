#include "app.h"
#include "app_state.h"

static app_state_t current_state;

app_state_t* app_get_state(void) {
    return &current_state;
}

esp_err_t app_init(void) {
    // telemetrum_init();
    // easymini_init();
    // init_servo();
    // init_recovery();
    // init_console();
    init_spi();
    init_logger_task();
    init_app_state_test();

    return ESP_OK;
}
