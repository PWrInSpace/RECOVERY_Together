#include "logger_config.h"

static const char *TAG = "LOGGER CONFIG";

static char data_buffer[1024];
static char frame_buffer[1024];

static logger_task_config_t logger_config = {
    .filename = "LOG",
    .log_path = "/",
    .log_path_size = 9,
    .core_id = 1,
    .stack_depth = 8100,
    .priority = 5,
    .mutex = NULL, // lub odpowiedni wskaźnik na mutex, jeśli jest wymagany
    .data_queue_size = 20,
    .data_item_size = 5,
    .data_drop_value = 10,
    .data_buffer = data_buffer,
    .data_buffer_size = sizeof(data_buffer),
    .frame_buffer = frame_buffer,
    .frame_buffer_size = sizeof(frame_buffer),

    .create_sd_frame_fnc = NULL,
    .create_sd_header_fnc = NULL,

    // mutex
    .spi_mutex = NULL
};

static sd_card_config_t sd_card_config = {
    .host = SPI2_HOST,
    .mount_config = {
        .format_if_mount_failed = true,
        .max_files = 4,
        .allocation_unit_size = 16 * 1024
    },
    .mount_point = "/sdcard",
    .cs_pin = 18
};

esp_err_t init_logger_task(void) {
    if (init_logger(&logger_config, &sd_card_config, &logger_task) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize logger task");
        return ESP_FAIL;
    }
    return ESP_OK;
}