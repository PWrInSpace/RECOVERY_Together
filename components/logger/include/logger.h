#ifndef LOGGER_H
#define LOGGER_H

#include "sd_card.h"
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
    const char *filename;
    char *log_path;
    size_t log_path_size;
    sd_card_config_t sd_card_config;

    uint32_t stack_depth;
    BaseType_t core_id;
    UBaseType_t priority;
    SemaphoreHandle_t mutex;

    error_handler error_handler_fnc;
    create_sd_frame create_sd_frame_fnc;
    create_sd_header create_sd_header_fnc;
    get_sd_header_size get_sd_header_size_fnc;

    size_t data_queue_size;
    size_t date_item_size;
} logger_task_config_t;

typedef struct {
    logger_task_config_t config;

    sd_card_t sd_card;

    TaskHandle_t task_handle;
    QueueHandle_t data_queue;
    SemaphoreHandle_t spi_mutex;
} logger_task_t;

esp_err_t init_logger(const logger_task_config_t *config, logger_task_t *logger_task);

esp_err_t terminate_logger(void);

#endif
