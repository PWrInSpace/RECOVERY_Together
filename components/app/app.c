#include "app.h"
#include "app_state.h"

typedef esp_err_t (*init_fnc)(void);

static void init_check(const init_fnc fnc) {
    if (fnc() != ESP_OK) {
        esp_restart();
    }
}

esp_err_t app_init(void) {
    init_check(init_spi);
    init_check(init_telemetrum);
    init_check(init_easymini);
    init_check(init_servo);
    init_check(init_recovery);
    init_check(init_console);
    init_check(init_logger_task);
    init_check(init_app_state);
    init_check(init_commands);

    return ESP_OK;
}
