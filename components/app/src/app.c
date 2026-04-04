#include <app.h>
#include <app_ui.h>

#include <styles/text.h>
#include <styles/colors.h>

void app_create() {
    app_initialize_ui();
    app_ui_create_ui_elements();
}

void app_initialize_ui() {
    initialize_text_styles();
    initialize_color_styles();
}
