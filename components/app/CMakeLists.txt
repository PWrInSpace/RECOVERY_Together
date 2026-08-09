idf_component_register(SRCS "app.c" "console_config.c" "cots_config.c" "recovery_config.c" "servo_config.c" "logger_config.c" "spi_config.c" "app_state.c" "i2c_config.c"
                    INCLUDE_DIRS "include"
                    REQUIRES sys cots recovery servo cli logger esp_driver_gpio
)