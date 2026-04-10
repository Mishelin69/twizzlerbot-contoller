idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES 
        wifi
        app_storage
        app
        espnow
        lvgl_port
)
