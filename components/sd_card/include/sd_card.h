#ifndef SD_CARD_H
#define SD_CARD_H

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define TAG "SD_CARD"
#define MOUNT_PATH "/sdcard"

typedef struct {
    spi_host_device_t host;
    esp_vfs_fat_mount_config_t mount_config;
    const char *mount_point;
    uint8_t cs_pin;
} sd_card_config_t;

typedef struct {
    sd_card_config_t config;
    sdmmc_card_t *card;
    bool mounted;
} sd_card_t;

esp_err_t SD_mount(const sd_card_config_t *config, sd_card_t *card);

esp_err_t SD_unmount(sd_card_t *card);

esp_err_t SD_remount(sd_card_t *card);

esp_err_t SD_write(const sd_card_t *card, const char *path, const char *data);

bool SD_file_exists(const sd_card_t *card, const char *path);

esp_err_t SD_create_file_path(const sd_card_t *card, const char *filename, char *path, const size_t path_len);

#endif
