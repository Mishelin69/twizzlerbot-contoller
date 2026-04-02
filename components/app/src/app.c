#include "display/lv_display.h"
#include <app.h>
#include <colors.h>

void app_create() {
    initialize_color_styles();
    app_create_background(lv_screen_active());
}

void app_create_background(lv_obj_t* container) {
    lv_obj_set_style_bg_color(container, background_grey, 0);
}
