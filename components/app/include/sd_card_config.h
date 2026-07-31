#ifndef SD_CARD_CONFIG_H
#define SD_CARD_CONFIG_H

#include "sd_card.h"

static sd_card_config_t sd_card_config = {
    .host = SPI2_HOST,
    .mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    },
    .mount_point = "/sdcard",
    .cs_pin = GPIO_NUM_25
};

#endif
