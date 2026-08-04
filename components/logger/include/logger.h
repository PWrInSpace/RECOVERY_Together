#ifndef LOGGER_H
#define LOGGER_H

#include "sd_card.h"
#include "spi.h"
#include "freertos/task.h"

typedef size_t (*create_sd_frame)(char *buffer, size_t buffer_size, void* data, size_t size);
typedef size_t (*create_sd_header)(char *buffer, size_t buffer_size, void* data, size_t size);

typedef struct {
    // sd card config
    const char *filename;
    char *log_path;
    size_t log_path_size;

    // thread config
    BaseType_t core_id;
    uint32_t stack_depth;
    UBaseType_t priority;

    // data queue config
    QueueHandle_t data_queue;
    size_t data_item_size;
    size_t data_drop_value;

    // buffers
    void *data_buffer;
    size_t data_buffer_size;
    char *frame_buffer;
    size_t frame_buffer_size;

    // callback functions config
    create_sd_frame create_sd_frame_fnc;
    create_sd_header create_sd_header_fnc;

    // mutex
    SemaphoreHandle_t *spi_mutex;
} logger_task_config_t;

typedef struct {
    logger_task_config_t config;

    sd_card_t sd_card;
    FILE *log_file;

    TaskHandle_t task_handle;
} logger_task_t;

esp_err_t init_logger(const logger_task_config_t *logger_task_config, const sd_card_config_t *sd_card_config, logger_task_t *logger_task);

esp_err_t terminate_logger(logger_task_t *logger_task);

esp_err_t logger_write(const logger_task_t *logger_task, const void *data, size_t data_size);

#endif
