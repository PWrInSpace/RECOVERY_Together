#include "sd_card.h"

static sdmmc_host_t host = SDMMC_HOST_DEFAULT();
static sdmmc_card_t *card;

esp_err_t init_sd_card() {
    if (sdmmc_host_init() != ESP_OK) {
        return ESP_FAIL;
    }

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    if (sdmmc_host_init_slot(SDMMC_HOST_SLOT_1, &slot_config) != ESP_OK) {
        return ESP_FAIL;
    }

    if (sdmmc_card_init(&host, card) != ESP_OK) {
        return ESP_FAIL;
    }

    esp_vfs_fat_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 3,
        .allocation_unit_size = 0,
        .disk_status_check_enable = false
    };
    if (esp_vfs_fat_sdmmc_mount(MOUNT_PATH, &host, &slot_config, &mount_config, &card) != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t write_to_sd_card(const char* path, const char* data, size_t length) {
    FILE *file = fopen(path, "a");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }

    size_t written = fprintf(file, "%s", data);
    fclose(file);

    if (written < length) {
        ESP_LOGE(TAG, "Failed to write data to file");
        return ESP_FAIL;
    }

    return ESP_OK;
}
