#include "commands.h"

static const char *TAG = "CMD";

esp_err_t process_command(const uint32_t command_id, const command_t *commands, const size_t commands_count) {
    for (size_t i = 0; i < commands_count; i++) {
        if (commands[i].command_id == command_id) {
            if (commands[i].command_fnc != NULL) {
                commands[i].command_fnc();
            } else {
                ESP_LOGE(TAG, "Command function is NULL");
                return ESP_FAIL;
            }
            return ESP_OK;
        }
    }
    return ESP_FAIL;
}