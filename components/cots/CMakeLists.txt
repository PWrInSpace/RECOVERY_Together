idf_component_register(SRCS "cots.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_driver_gpio config log)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format" "-Wall" "-Werror")
