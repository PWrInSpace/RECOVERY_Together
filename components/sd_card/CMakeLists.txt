idf_component_register(SRCS "sd_card.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_driver_sdmmc sdmmc fatfs
)
