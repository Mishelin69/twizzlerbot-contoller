idf_component_register(
    SRCS
        src/fonts/siemens_sans_bold_big.c
        src/fonts/siemens_sans_global_bold.c
        src/fonts/siemens_sans_global_roman.c
        src/fonts/siemens_sans_next_reservation_text.c
        src/fonts/siemens_sans_registration_text.c

        img_files/alien_transparent_background.c

        src/styles/colors.c
        src/styles/text.c
        src/styles/images.c
        src/styles/styles.c

        src/input/button.c

        src/page_manager.c
        src/page_vehicle.c
        src/page_sensor.c

    INCLUDE_DIRS
        include/
    REQUIRES 
        lvgl_port 
)
