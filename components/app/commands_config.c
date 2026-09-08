#include "commands_config.h"
#include "app_state.h"
#include <string.h>

#define COMMAND_QUEUE_SIZE 10
#define COMMAND_SIZE sizeof(uint32_t)

static const char *TAG = "COMMANDS CONFIG";

static TaskHandle_t command_task;
static uint8_t queue_storage_buffer[COMMAND_QUEUE_SIZE * COMMAND_SIZE];
static StaticQueue_t queue_buffer;

static void easymini_arm(void) {
    cots_arm(&easymini);
}

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

static QueueHandle_t command_queue = NULL;
static uint8_t i2c_buffer[sizeof(struct command)];

static bool process_command_i2c(const uint8_t* data) {
    const i2c_command_t *cmd = (i2c_command_t*)data;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (command_queue != NULL) {
        xQueueSendFromISR(command_queue, &cmd->cmd.command, &xHigherPriorityTaskWoken);
    }

    return xHigherPriorityTaskWoken == pdTRUE;
}

static void commands_task(void *arg) {
    uint32_t cmd;
    while (1) {
        if (xQueueReceive(command_queue, &cmd, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Received command: %lu", cmd);
            process_command(cmd, commands, sizeof(commands) / sizeof(command_t));
            memset(i2c_buffer, 0, sizeof(i2c_buffer));
            i2c_read(&i2c);
        }
    }
}

static sys_i2c_config_t i2c_config = {
    .port = I2C_NUM_0,
    .sda_pin = GPIO_NUM_14,
    .scl_pin = GPIO_NUM_27,
    .slave_addr = 0x0B,
    .tx_buffer_size = sizeof(i2c_data_t),
    .rx_buffer_size = sizeof(i2c_command_t),
    .rx_buffer = i2c_buffer,
    .receive_callback = process_command_i2c,
};

esp_err_t init_commands() {
    command_queue = xQueueCreateStatic(COMMAND_QUEUE_SIZE, COMMAND_SIZE, queue_storage_buffer, &queue_buffer);

    if (command_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create command queue");
        return ESP_FAIL;
    }

    if (xTaskCreatePinnedToCore(
        commands_task,
        "commands_task",
        4096,NULL,
        5,
        &command_task,
        0
        ) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create commands task");
        return ESP_FAIL;
    }

    if (i2c_init(&i2c_config, &i2c) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C");
        return ESP_FAIL;
    }

    if (i2c_read(&i2c) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start I2C read");
        return ESP_FAIL;
    }

    return ESP_OK;
}
