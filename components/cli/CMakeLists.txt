idf_component_register( SRCS "console.c" "console_config.c"
                    INCLUDE_DIRS "." 
                    REQUIRES console servo recovery_control config cots data PressureSensor i2c)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format" "-Wall" "-Werror")