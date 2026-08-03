idf_component_register( SRCS "console.c" "../app/console_config.c"
                    INCLUDE_DIRS "."
                    REQUIRES console servo recovery cots sys
)