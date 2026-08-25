#ifndef COTS_H
#define COTS_H 

#include "driver/gpio.h"
#include "esp_log.h"
#include "recovery.h"
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define DISARMED 0
#define ARMED 1
#define COTS_EVENT_QUEUE_SIZE 10

typedef enum {
    COTS_EVENT_APOGEE,
    COTS_EVENT_MAIN
} cots_event_t;

typedef struct {
    gpio_num_t arming_pin;
    gpio_num_t apogee_pin;
    gpio_num_t main_pin;
    recovery_config_t *recovery;
} cots_config_t;

typedef struct {
    bool armed: 1;
    bool apogee_detected: 1;
    bool first_stage: 1;
    bool second_stage: 1;
} cots_data_t;

typedef struct {
    cots_config_t config;
    cots_data_t data;
    QueueHandle_t event_queue;
    StaticQueue_t event_queue_buffer;
    uint8_t event_queue_storage[COTS_EVENT_QUEUE_SIZE * sizeof(cots_event_t)];
    TaskHandle_t task_handle;
} cots_t;

esp_err_t cots_init(const cots_config_t *cots_config, cots_t *cots);

esp_err_t cots_arm(cots_t *cots);

esp_err_t cots_disarm(cots_t *cots);

esp_err_t apogee_check(cots_t *cots);

#endif