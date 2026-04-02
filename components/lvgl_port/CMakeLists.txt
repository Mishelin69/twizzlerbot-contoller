idf_component_register(
    SRCS
        src/waveshare_rgb_lcd_port.c
        src/lvgl_v9_port.cpp
    INCLUDE_DIRS
        include/
        .
    REQUIRES 
        lvgl
        esp_driver_gpio
        esp_lcd
        esp_timer
        driver
        espressif__esp_lcd_touch_gt911
)

# Tell LVGL to use LV_CONF_INCLUDE_SIMPLE
idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_definitions(${lvgl_lib} PRIVATE LV_CONF_INCLUDE_SIMPLE=1)

# Add only the folder containing lv_conf.h
target_include_directories(${lvgl_lib} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Optional: suppress format warnings from LVGL
target_compile_options(${lvgl_lib} PRIVATE -Wno-format)
