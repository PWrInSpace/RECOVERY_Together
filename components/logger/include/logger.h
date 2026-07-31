#ifndef LOGGER_H
#define LOGGER_H

#include "sd_card.h"
#include "spi.h"
#include "freertos/task.h"

typedef enum {
    SD_INIT,
    SD_QUEUE_READ,
    SD_WRITE,
    SD_MUTEX,
} LOGGER_TASK_ERROR;

typedef void (*error_handler)(LOGGER_TASK_ERROR error_code);
typedef size_t (*create_sd_frame)(char *buffer, size_t buffer_size, void* data, size_t size);
typedef size_t (*create_sd_header)(char *buffer, size_t buffer_size, void* data, size_t size);
typedef size_t (*get_sd_header_size)(void);

typedef struct {
    // sd card config
    const char *filename;
    char *log_path;
    size_t log_path_size;

    // thread config
    BaseType_t core_id;
    uint32_t stack_depth;
    UBaseType_t priority;
    SemaphoreHandle_t mutex;
    int task_delay_ms;

    // data queue config
    size_t data_queue_size;
    size_t data_item_size;
    size_t data_drop_value;
    uint8_t *data_buffer;
    char *frame_buffer;

    // callback functions config
    error_handler error_handler_fnc;
    create_sd_frame create_sd_frame_fnc;
    create_sd_header create_sd_header_fnc;
    get_sd_header_size get_sd_header_size_fnc;

    // mutex
    SemaphoreHandle_t spi_mutex;
} logger_task_config_t;

typedef struct {
    logger_task_config_t config;

    sd_card_t sd_card;
    FILE *log_file;

    TaskHandle_t task_handle;
    QueueHandle_t data_queue;
} logger_task_t;

esp_err_t init_logger(const logger_task_config_t *logger_task_config, const sd_card_config_t *sd_card_config, logger_task_t *logger_task);

esp_err_t terminate_logger(logger_task_t *logger_task);

#endif
