#ifndef SD_CARD_H
#define SD_CARD_H

#include "esp_vfs_fat.h"

#define TAG "SD_CARD"
#define MOUNT_PATH "/sdcard"

typedef struct {
    spi_host_device_t host;
    esp_vfs_fat_mount_config_t mount_config;
    const char *mount_point;
    uint8_t cs_pin;
    uint8_t cd_pin;
} sd_card_config_t;

typedef struct {
    sd_card_config_t config;
    sdmmc_card_t *card;
    bool mounted;
} sd_card_t;

esp_err_t init_sd_card(const sd_card_config_t *config, sd_card_t *card);

esp_err_t deinit_sd_card(sd_card_t *card);

#endif
