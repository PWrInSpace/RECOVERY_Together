#include "logger.h"

#include <sys/unistd.h>

static const char* TAG = "LOGGER";

static esp_err_t save_data(const logger_task_t *logger) {
    while (uxQueueMessagesWaiting(logger->data_queue) > 0) {
        if (xQueueReceive(logger->data_queue, logger->config.data_buffer, 0) == pdFALSE) {
            ESP_LOGE(TAG, "Unable to read data from queue");
            return ESP_FAIL;
        }

        const size_t written = logger->config.create_sd_frame_fnc(
            logger->config.frame_buffer, 
            logger->config.data_frame_size, 
            logger->config.data_buffer, 
            logger->config.data_item_size
        );
        if (written != logger->config.data_item_size) {
            ESP_LOGE(TAG, "Unable to create sd frame");
        }

        fwrite(logger->config.frame_buffer, sizeof(char), written, logger->log_file);
    }
    fflush(logger->log_file);

    return ESP_OK;
}

static esp_err_t data_check(const logger_task_t *logger) {
    if (uxQueueMessagesWaiting(logger->data_queue) < logger->config.data_drop_value) {
        ESP_LOGI(TAG, "Queue drop value not reached");
        return ESP_FAIL;
    }

    if (save_data(logger) != ESP_OK) {
        ESP_LOGE(TAG, "Unable to save data");
        return ESP_FAIL;
    }

    return ESP_OK;
}

static void sd_task(void *args) {
    const logger_task_t *logger = (logger_task_t *)args;
    ESP_LOGI(TAG, "Starting logger task");
    while (true) {
        if (xSemaphoreTake(logger->config.spi_mutex, 10) == pdTRUE) {
            data_check(logger);
            xSemaphoreGive(logger->config.spi_mutex);
        } else {
            // error
        }
        vTaskDelay(pdMS_TO_TICKS(logger->config.task_delay_ms));
    }
}

static esp_err_t init_sd_card(logger_task_t *logger_task, const sd_card_config_t *sd_card_config) {
    if (SD_mount(sd_card_config, &logger_task->sd_card) != ESP_OK) {
        ESP_LOGE(TAG, "Unable to initialize SD card");
        return ESP_FAIL;
    }

    return ESP_OK;
}

static esp_err_t init_file(logger_task_t *logger_task) {
    if (SD_create_file_path(&logger_task->sd_card, logger_task->config.filename, logger_task->config.log_path, logger_task->config.log_path_size) != ESP_OK) {
        ESP_LOGE(TAG, "Unable to create file %s", logger_task->config.filename);
        return ESP_FAIL;
    }

    logger_task->log_file = fopen(logger_task->config.log_path, "a");
    if (logger_task->log_file == NULL) {
        ESP_LOGE(TAG, "Unable to open file %s", logger_task->config.filename);
        return ESP_FAIL;
    }

    return ESP_OK;
}

static esp_err_t init_task(logger_task_t *logger_task) {
    logger_task->data_queue = xQueueCreate(logger_task->config.data_queue_size, logger_task->config.data_item_size);
    if (logger_task->data_queue == NULL) {
        ESP_LOGE(TAG, "Unable to create data queue");
        return ESP_FAIL;
    }

    BaseType_t res = xTaskCreatePinnedToCore(
        sd_task,
        "logger task",
        logger_task->config.stack_depth,
        logger_task,
        logger_task->config.priority,
        &logger_task->task_handle,
        logger_task->config.core_id
    );
    if (res != pdPASS) {
        ESP_LOGE(TAG, "Unable to create task handle");
        vQueueDelete(logger_task->data_queue);
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t init_logger(const logger_task_config_t *logger_task_config, const sd_card_config_t *sd_card_config, logger_task_t *logger_task) {
    logger_task->config = *logger_task_config;

    if (init_sd_card(logger_task, sd_card_config) != ESP_OK) {
        ESP_LOGE(TAG, "Unable to initialize SD card");
        return ESP_FAIL;
    }

    if (init_file(logger_task) != ESP_OK) {
        ESP_LOGE(TAG, "Unable to initialize file %s", logger_task->config.filename);
        return ESP_FAIL;
    }

    if (init_task(logger_task) != ESP_OK) {
        ESP_LOGE(TAG, "Unable to initialize logger task");
        fclose(logger_task->log_file);
        return ESP_FAIL;
    }

    return ESP_OK;
}