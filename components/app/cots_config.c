#include "cots_config.h"

static const char *TAG = "COTS CONFIG";

cots_t telemetrum;
cots_t bluejay;

static StaticSemaphore_t telemetrum_mutex_buffer;
static StaticQueue_t telemetrum_event_queue_buffer;
static uint8_t telemetrum_event_queue_storage[COTS_EVENT_QUEUE_SIZE * sizeof(cots_event_t)];

static StaticSemaphore_t bluejay_mutex_buffer;
static StaticQueue_t bluejay_event_queue_buffer;
static uint8_t bluejay_event_queue_storage[COTS_EVENT_QUEUE_SIZE * sizeof(cots_event_t)];

static cots_config_t bluejay_config = {
    .arming_pin = GPIO_NUM_2,
    .apogee_pin = GPIO_NUM_18,
    .main_pin = GPIO_NUM_21,
    .mutex_buffer = &bluejay_mutex_buffer,
    .event_queue_buffer = &bluejay_event_queue_buffer,
    .event_queue_storage = bluejay_event_queue_storage
};

static cots_config_t telemetrum_config = {
    .arming_pin = GPIO_NUM_13,
    .apogee_pin = GPIO_NUM_19,
    .main_pin = GPIO_NUM_35,
    .mutex_buffer = &telemetrum_mutex_buffer,
    .event_queue_buffer = &telemetrum_event_queue_buffer,
    .event_queue_storage = telemetrum_event_queue_storage
};

esp_err_t init_telemetrum(void) {
    telemetrum_config.recovery = &recovery;
    if (cots_init(&telemetrum_config, &telemetrum) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize telemetrum");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t init_bluejay(void) {
    bluejay_config.recovery = &recovery;
    if (cots_init(&bluejay_config, &bluejay) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bluejay");
        return ESP_FAIL;
    }
    return ESP_OK;
}
