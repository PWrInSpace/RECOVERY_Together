idf_component_register(SRCS "spi.c" "i2c.c" "adc.c"
        INCLUDE_DIRS "include"
        REQUIRES esp_driver_spi esp_driver_i2c esp_adc
)
