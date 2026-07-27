idf_component_register(SRCS "recovery.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_driver_gpio config cots esp_timer servo)
