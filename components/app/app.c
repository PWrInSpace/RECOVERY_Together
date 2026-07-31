#include "app.h"

static const spi_config_t spi_config = {
    .host_device = SPI2_HOST,
    .mosi_pin = GPIO_NUM_25,
    .miso_pin = GPIO_NUM_32,
    .sck_pin = GPIO_NUM_33
};

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

static recovery_config_t recovery_config = {
    .separation_1_pin = GPIO_NUM_34,
    .separation_2_pin = GPIO_NUM_23,
    .first_stage_pin = GPIO_NUM_4,
    .second_stage_pin = GPIO_NUM_17,
};

static cots_config_t easymini_config = {
    .arming_pin = GPIO_NUM_26,
    .apogee_pin = GPIO_NUM_27,
    .main_pin = GPIO_NUM_14
};

static cots_config_t telemetrum_config = {
    .arming_pin = GPIO_NUM_29,
    .apogee_pin = GPIO_NUM_28,
    .main_pin = GPIO_NUM_16,
};

