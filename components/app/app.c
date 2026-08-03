#include "app.h"

esp_err_t app_init(void) {
    telemetrum_init();
    easymini_init();
    init_servo();
    init_recovery();
    init_console();
    init_logger_task();

    return ESP_OK;
}
