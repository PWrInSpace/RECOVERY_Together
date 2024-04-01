idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ""
                    REQUIRES config cots data PressureSensor driver recovery_control)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format" "-Wall" "-Werror")