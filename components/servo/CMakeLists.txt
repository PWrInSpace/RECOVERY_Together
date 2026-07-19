idf_component_register(SRCS "servo_control.c"
                    INCLUDE_DIRS "."
                    REQUIRES config esp_timer esp_driver_mcpwm esp_driver_gpio)