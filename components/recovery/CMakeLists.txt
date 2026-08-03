idf_component_register(SRCS "recovery.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_driver_gpio cots esp_timer servo)
