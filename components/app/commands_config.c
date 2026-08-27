#include "commands_config.h"
#include "app_state.h"

static const char *TAG = "COMMANDS CONFIG";

static void easymini_arm(void) {
    cots_arm(&easymini);
};

static void easymini_disarm(void) {
    cots_disarm(&easymini);
}

static void telemetrum_arm(void) {
    cots_arm(&telemetrum);
}

static void telemetrum_disarm(void) {
    cots_disarm(&telemetrum);
}

static void force_first_stage(void) {
    first_stage_deploy(&recovery);
}

static void force_second_stage(void) {
    second_stage_deploy(&recovery);
}

static command_t commands[] = {
    {.command_id = EASYMINI_ARM_CMD, .command_fnc = easymini_arm},
    {.command_id = EASYMINI_DISARM_CMD, .command_fnc = easymini_disarm},
    {.command_id = TELEMETRUM_ARM_CMD, .command_fnc = telemetrum_arm},
    {.command_id = TELEMETRUM_DISARM_CMD, .command_fnc = telemetrum_disarm},
    {.command_id = FORCE_FIRST_STAGE_CMD, .command_fnc = force_first_stage},
    {.command_id = FORCE_SECOND_STAGE_CMD, .command_fnc = force_second_stage},
};

i2c_slave_t i2c;

static uint8_t i2c_buffer[1];

static bool process_command_i2c(const uint8_t* data) {
    ESP_LOGI(TAG, "Received command: %d", data[0]);
    if (process_command(data[0], commands, sizeof(commands) / sizeof(command_t)) != ESP_OK) {
        return false;
    }
    return true;
}

static sys_i2c_config_t i2c_config = {
    .port = I2C_NUM_0,
    .sda_pin = GPIO_NUM_14,
    .scl_pin = GPIO_NUM_27,
    .slave_addr = 0x0B,
    .tx_buffer_size = sizeof(i2c_data_t) * 100,
    .rx_buffer_size = 1,
    .rx_buffer = i2c_buffer,
    .receive_callback = process_command_i2c,
};

esp_err_t init_commands() {
    if (i2c_init(&i2c_config, &i2c) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C");
        return ESP_FAIL;
    }

    return ESP_OK;
}
