#include "cots.h"

static const char* TAG = "COTS";

static void cots_task(void* arg) {
    cots_t* cots = arg;
    cots_event_t event;

    while (1) {
        if (xQueueReceive(cots->event_queue, &event, portMAX_DELAY) == pdTRUE) {
            if (event == COTS_EVENT_APOGEE) {
                ESP_LOGI(TAG, "Apogee detected");
                cots->data.apogee_detected = true;
                cots->data.first_stage = true;
                if (cots->config.recovery && cots->config.recovery->first_stage) {
                    cots->config.recovery->first_stage();
                }
            } else if (event == COTS_EVENT_MAIN) {
                ESP_LOGI(TAG, "Main deployment detected");
                cots->data.second_stage = true;
                if (cots->config.recovery && cots->config.recovery->second_stage) {
                    cots->config.recovery->second_stage();
                }
            }
        }
    }
}

static void IRAM_ATTR gpio_apogee_handler(void* arg) {
    const cots_t *cots = arg;
    const cots_event_t event = COTS_EVENT_APOGEE;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (cots->event_queue) {
        xQueueSendFromISR(cots->event_queue, &event, &xHigherPriorityTaskWoken);
    }
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void IRAM_ATTR gpio_main_handler(void* arg) {
    const cots_t *cots = arg;
    const cots_event_t event = COTS_EVENT_MAIN;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (cots->event_queue) {
        xQueueSendFromISR(cots->event_queue, &event, &xHigherPriorityTaskWoken);
    }
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

esp_err_t cots_init(const cots_config_t *cots_config, cots_t *cots) {
    *cots = (cots_t){
        .config = *cots_config,
        .data = {0},
        .event_queue = NULL,
        .task_handle = NULL
    };

    ESP_LOGI(TAG,"Cots initialization");

    cots->event_queue = xQueueCreateStatic(COTS_EVENT_QUEUE_SIZE, sizeof(cots_event_t), cots->event_queue_storage, &cots->event_queue_buffer);
    if (cots->event_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create event queue");
        return ESP_FAIL;
    }

    if (xTaskCreate(cots_task, "cots_task", 4096, cots, 5, &cots->task_handle) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create cots task");
        return ESP_FAIL;
    }

    const gpio_config_t arming_output = {
        .pin_bit_mask = 1ULL << cots_config->arming_pin,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    const gpio_config_t apogee_input = {
        .pin_bit_mask = 1ULL << cots_config->apogee_pin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    const gpio_config_t main_input = {
        .pin_bit_mask = 1ULL << cots_config->main_pin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    if (gpio_config(&arming_output) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure arming output");
        return ESP_FAIL;
    }
    if (gpio_config(&apogee_input) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure apogee input");
        return ESP_FAIL;
    }
    if (gpio_config(&main_input) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure main input");
        return ESP_FAIL;
    }

    cots_disarm(cots);

    gpio_isr_handler_add(cots->config.apogee_pin, gpio_apogee_handler, cots);
    gpio_isr_handler_add(cots->config.main_pin, gpio_main_handler, cots);

    ESP_LOGI(TAG, "Cots initialized");

    return ESP_OK;
}

esp_err_t cots_arm(cots_t *cots){
    ESP_LOGI(TAG, "Cots arming");

    if (gpio_set_level(cots->config.arming_pin, 1) != ESP_OK) {
        ESP_LOGE(TAG,"Failed to arm");
        return ESP_FAIL;
    }
    cots->data.armed = ARMED;

    ESP_LOGI(TAG, "Cots arming done");

    return ESP_OK;
}

esp_err_t cots_disarm(cots_t *cots){
    ESP_LOGI(TAG,"Cots disarming");

    if (gpio_set_level(cots->config.arming_pin, 0) != ESP_OK) {
        ESP_LOGE(TAG,"Failed to disarm");
        return ESP_FAIL;
    }
    cots->data.armed = DISARMED;

    ESP_LOGI(TAG,"Cots disarming done");

    return ESP_OK;
}

esp_err_t apogee_check(cots_t *cots){
    if (gpio_get_level(cots->config.apogee_pin)) {
        cots->data.apogee_detected = true;
    } else {
        cots->data.apogee_detected = false;
    }

    return ESP_OK;
}
