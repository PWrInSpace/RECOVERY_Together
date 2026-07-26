#include "sd_card.h"

#include <sys/stat.h>

esp_err_t SD_mount(const sd_card_config_t *config, sd_card_t *card) {
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = config->cs_pin;
    slot_config.gpio_cd = config->cd_pin;
    slot_config.host_id = config->host;

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = config->host;

    card->config = *config;

    if (esp_vfs_fat_sdspi_mount(config->mount_point, &host, &slot_config, &config->mount_config, &card->card) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount SD card.");
        return ESP_FAIL;
    }
    card->mounted = true;
    return ESP_OK;
}

esp_err_t SD_unmount(sd_card_t *card) {
    if (card->mounted) {
        if (esp_vfs_fat_sdcard_unmount(card->config.mount_point, card->card) != ESP_OK) {
            ESP_LOGE(TAG, "Failed to unmount SD card.");
            return ESP_FAIL;
        }
        card->mounted = false;
    }
    return ESP_OK;
}

esp_err_t SD_remount(sd_card_t *card) {
    if (SD_unmount(card) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to unmount SD card for remounting.");
        return ESP_FAIL;
    }
    if (SD_mount(&card->config, card) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to remount SD card.");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t SD_write(const sd_card_t *card, const char *path, const char *data) {
    if (!card->mounted) {
        ESP_LOGE(TAG, "SD card not mounted.");
        return ESP_FAIL;
    }

    if (sdmmc_get_status(card->card) != ESP_OK) {
        ESP_LOGE(TAG, "SD card error.");
        return ESP_FAIL;
    }

    FILE *file = fopen(path, "a");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing.");
        return ESP_FAIL;
    }

    const int written = fprintf(file, "%s", data);
    if (written < 0) {
        ESP_LOGE(TAG, "Failed to write to file.");
        fclose(file);
        return ESP_FAIL;
    }

    fclose(file);
    return ESP_OK;
}

bool SD_file_exists(const sd_card_t *card, const char *path) {
    if (!card->mounted) {
        ESP_LOGE(TAG, "SD card not mounted.");
        return false;
    }

    struct stat st;
    if (stat(path, &st) != 0) {
        return true;
    }
    return false;
}

esp_err_t SD_create_file_path(const sd_card_t *card, const char *filename, char *path, const size_t path_len) {
    if (!card->mounted) {
        ESP_LOGE(TAG, "SD card not mounted.");
        return ESP_FAIL;
    }

    for (int i = 1; i < 1000; i++) {
        snprintf(path, path_len, "%s/%s_%03d", card->config.mount_point, filename, i);
        if (!SD_file_exists(card, path)) {
            return ESP_OK;
        }
    }

    ESP_LOGE(TAG, "No free file index found. Limit reached.");
    return ESP_FAIL;
}