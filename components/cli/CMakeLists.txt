idf_component_register( SRCS "console.c" "console_config.c"
                    INCLUDE_DIRS "." 
                    REQUIRES console servo recovery config cots sys)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format" "-Wall" "-Werror")