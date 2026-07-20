#ifndef SD_CARD_H
#define SD_CARD_H

#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "stdio.h"

#define TAG "SD_CARD"

#define MOUNT_PATH "/sdcard"

esp_err_t init_sd_card();

esp_err_t write_to_sd_card(const char* path, const char* data, size_t length);

#endif