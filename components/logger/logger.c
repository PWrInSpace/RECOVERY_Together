#include "logger.h"

#include <sys/unistd.h>

static const char* TAG = "LOGGER";

static esp_err_t save_data(const logger_task_t *logger_task) {
    while (uxQueueMessagesWaiting(logger_task->config.data_queue) > 0) {
        if (xQueueReceive(logger_task->config.data_queue, logger_task->config.data_buffer, 0) == pdFALSE) {
            ESP_LOGE(TAG, "Unable to read data from queue");
            return ESP_FAIL;
        }

        const size_t written = logger_task->config.create_sd_frame_fnc(
            logger_task->config.frame_buffer,
            logger_task->config.frame_buffer_size,
            logger_task->config.data_buffer,
            logger_task->config.data_buffer_size
        );
        if (written == 0) {
            ESP_LOGE(TAG, "Unable to create sd frame");
        }

        fwrite(logger_task->config.frame_buffer, sizeof(char), written, logger_task->log_file);
    }
    fflush(logger_task->log_file);
    fsync(fileno(logger_task->log_file));

    return ESP_OK;
}

static bool data_check(const logger_task_t *logger_task) {
    if (uxQueueMessagesWaiting(logger_task->config.data_queue) < logger_task->config.data_drop_value) {
        return false;
    }
    return true;
}

static void process_logger_step(const logger_task_t *logger_task) {
    if (xSemaphoreTake(*logger_task->config.spi_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        if (save_data(logger_task) != ESP_OK) {
            ESP_LOGE(TAG, "Unable to save data");
        }
        xSemaphoreGive(*logger_task->config.spi_mutex);
    } else {
        ESP_LOGE(TAG, "Unable to take spi mutex");
    }
}

static void sd_task(void *args) {
    const logger_task_t *logger_task = (logger_task_t *)args;
    ESP_LOGI(TAG, "Starting logger task");
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        process_logger_step(logger_task);
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

    // todo dodać pisanie headera

    return ESP_OK;
}

static esp_err_t init_task(logger_task_t *logger_task) {
    if (logger_task->config.data_queue == NULL) {
        ESP_LOGE(TAG, "Unable to create data queue");
        return ESP_FAIL;
    }

    const BaseType_t res = xTaskCreatePinnedToCore(
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
        vQueueDelete(logger_task->config.data_queue);
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t init_logger(const logger_task_config_t *logger_task_config, const sd_card_config_t *sd_card_config, logger_task_t *logger_task) {
    ESP_LOGI(TAG, "Starting logger task");
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

esp_err_t terminate_logger(logger_task_t *logger_task) {
    ESP_LOGI(TAG, "Terminating logger task");

    if (logger_task->task_handle != NULL) {
        vTaskDelete(logger_task->task_handle);
        logger_task->task_handle = NULL;
    }

    if (logger_task->log_file != NULL) {
        fclose(logger_task->log_file);
        logger_task->log_file = NULL;
    }

    SD_unmount(&logger_task->sd_card);

    return ESP_OK;
}

esp_err_t logger_write(const logger_task_t *logger_task, const void *data, const size_t data_size) {
    if (logger_task == NULL || logger_task->config.data_queue == NULL) {
        return ESP_FAIL;
    }

    if (data_size != logger_task->config.data_item_size) {
        ESP_LOGE(TAG, "Data size mismatch: expected %zu, got %zu", logger_task->config.data_item_size, data_size);
        return ESP_FAIL;
    }

    if (logger_task->sd_card.mounted == false) {
        ESP_LOGE(TAG, "SD card is not mounted");
        return ESP_FAIL;
    }

    if (xQueueSend(logger_task->config.data_queue, data, 0) == pdFALSE) {
        ESP_LOGE(TAG, "Unable to add data to queue");
        return ESP_FAIL;
    }

    if (data_check(logger_task)) {
        xTaskNotifyGive(logger_task->task_handle);
    }

    return ESP_OK;
}