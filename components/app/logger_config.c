#include "logger_config.h"
#include "spi_config.h"
#include "esp_timer.h"
#include <stdio.h>

#define DATA_QUEUE_SIZE 20
#define DATA_ITEM_SIZE sizeof(float)

logger_task_t logger_task;

static const char *TAG = "LOGGER CONFIG";

static char data_buffer[1024];
static char frame_buffer[1024];
static uint8_t queue_storage_buffer[DATA_QUEUE_SIZE * DATA_ITEM_SIZE];
static StaticQueue_t queue_buffer;

static size_t test_create_sd_frame(char *buffer, size_t buffer_size, void* data, size_t size) {
    if (data == NULL || size < sizeof(float)) {
        return 0;
    }
    float current_time = *(float*)data;
    return snprintf(buffer, buffer_size, "TIME: %.2f\n", current_time);
}

static char log_path_buffer[64];

static logger_task_config_t logger_config = {
    .filename = "LOG",
    .log_path = log_path_buffer,
    .log_path_size = sizeof(log_path_buffer),
    .core_id = 1,
    .stack_depth = 8100,
    .priority = 5,
    .data_item_size = DATA_ITEM_SIZE,
    .data_drop_value = 10,
    .data_buffer = data_buffer,
    .data_buffer_size = sizeof(data_buffer),
    .frame_buffer = frame_buffer,
    .frame_buffer_size = sizeof(frame_buffer),
    .create_sd_frame_fnc = test_create_sd_frame,
    .create_sd_header_fnc = NULL,
};

static sd_card_config_t sd_card_config = {
    .host = SPI2_HOST,
    .mount_config = {
        .format_if_mount_failed = true,
        .max_files = 4,
        .allocation_unit_size = 16 * 1024
    },
    .mount_point = "/sdcard",
    .cs_pin = GPIO_NUM_15
};

esp_err_t init_logger_task(void) {
    logger_config.spi_mutex = &spi_mutex;
    logger_config.data_queue = xQueueCreateStatic(DATA_QUEUE_SIZE, DATA_ITEM_SIZE, queue_storage_buffer, &queue_buffer);
    if (logger_config.data_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create data queue");
        return ESP_FAIL;
    }

    if (init_logger(&logger_config, &sd_card_config, &logger_task) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize logger task");
        return ESP_FAIL;
    }
    return ESP_OK;
}