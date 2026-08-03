idf_component_register(SRCS "app.c" "console_config.c" "cots_config.c" "recovery_config.c" "servo_config.c"
                    INCLUDE_DIRS "include"
                    REQUIRES sys cots recovery servo cli
)