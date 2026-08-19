#include "app.h"
#include "app_state.h"

esp_err_t app_init(void) {
    init_spi();
    init_telemetrum();
    init_easymini();
    init_servo();
    init_recovery();
    init_console();
    init_logger_task();
    init_app_state();
    init_commands();

    return ESP_OK;
}
