idf_component_register(SRCS "spi/spi.c" "i2c/i2c.c"
        INCLUDE_DIRS "include"
        REQUIRES esp_driver_spi driver config recovery
)
