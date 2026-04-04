#include "page_manager.h"
#include <app_ui.h>

#include <lvgl.h>

#include <styles/styles.h>
#include <styles/images.h>
#include <styles/colors.h>

static lv_obj_t* MASTER_CONTAINER = NULL;

lv_obj_t* app_ui_create_background(lv_obj_t* container) {
    lv_obj_t* background_container = style_create_default_object(container, 0);

    lv_obj_set_style_bg_opa(background_container, LV_OPA_COVER, 0);
    lv_obj_remove_flag(background_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(background_container, background_grey, 0);
    lv_obj_align(background_container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(background_container, lv_pct(100), lv_pct(100));

    return background_container;
}

lv_obj_t* app_ui_create_background_alien(lv_obj_t* container) {
    lv_obj_t* alien_img = create_alien_transparent(container);
    lv_obj_align(alien_img, LV_ALIGN_CENTER, 0, 0);
    return alien_img;
}

void app_ui_create_ui_elements() {
    MASTER_CONTAINER = app_ui_create_background(lv_screen_active());
    app_ui_create_background_alien(MASTER_CONTAINER);
    page_manager_create(MASTER_CONTAINER);
}
