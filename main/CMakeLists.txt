idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ""
                    REQUIRES config esp_driver_gpio cots recovery sys esp_adc servo cli)
