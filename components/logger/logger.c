#include "logger.h"

static void sd_task(void *args) {
    ESP_LOGI(TAG, "Starting logger task");
    while (true) {
        // todo napisać pisanie na kartę sd
    }
}

static esp_err_t init_sd_card(logger_task_t *logger_task) {
    if (SD_mount(&logger_task->config.sd_card_config, &logger_task->sd_card) != ESP_OK) {
        ESP_LOGE(TAG, "Unable to initialize SD card");
        return ESP_FAIL;
    }

    if (SD_create_file_path(&logger_task->sd_card, logger_task->config.filename, logger_task->config.log_path, logger_task->config.log_path_size) != ESP_OK) {
        ESP_LOGE(TAG, "Unable to create file %s", logger_task->config.filename);
        return ESP_FAIL;
    }

    return ESP_OK;
}

static esp_err_t init_task(logger_task_t *logger_task) {
    logger_task->data_queue = xQueueCreate(logger_task->config.data_queue_size, logger_task->config.date_item_size);
    if (logger_task->data_queue == NULL) {
        ESP_LOGE(TAG, "Unable to create data queue");
        return ESP_FAIL;
    }

    xTaskCreatePinnedToCore(
        sd_task,
        "logger task",
        logger_task->config.stack_depth,
        NULL,
        logger_task->config.priority,
        &logger_task->task_handle,
        logger_task->config.core_id
        );
    if (logger_task->task_handle == NULL) {
        ESP_LOGE(TAG, "Unable to create task handle");
        vQueueDelete(logger_task->data_queue);
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t init_logger(const logger_task_config_t *config, logger_task_t *logger_task) {
    logger_task->config = *config;

    if (init_sd_card(logger_task) != ESP_OK) {
        ESP_LOGE(TAG, "Unable to initialize SD card");
        return ESP_FAIL;
    }

    if (init_task(logger_task) != ESP_OK) {
        ESP_LOGE(TAG, "Unable to initialize logger task");
        return ESP_FAIL;
    }

    return ESP_OK;
}