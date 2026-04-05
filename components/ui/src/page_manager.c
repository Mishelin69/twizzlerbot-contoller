#include <page_manager.h>

#include <page_sensor.h>
#include <page_vehicle.h>

#include <lvgl.h>

#include <styles/colors.h>
#include <styles/text.h>

static lv_obj_t* TABVIEW = NULL;

void page_manager_create(lv_obj_t* container, uint8_t joystick_vehicle_id, uint8_t joystick_sensor_id) {
    TABVIEW = page_manager_create_tabview(container);
    page_manager_create_pages(TABVIEW, joystick_vehicle_id, joystick_sensor_id);

    lv_obj_t * tab_content = lv_tabview_get_content(TABVIEW);
    lv_obj_remove_flag(tab_content, LV_OBJ_FLAG_SCROLLABLE);
}

lv_obj_t* page_manager_create_tabview(lv_obj_t* container) {
    lv_obj_t* tabview =  lv_tabview_create(container);

    lv_obj_set_style_bg_opa(tabview, LV_OPA_TRANSP, 0);
    page_manager_tabview_set_top_bar_bg(tabview); 
 
    return tabview;
}

void page_manager_tabview_set_top_bar_bg(lv_obj_t* tabview) {
    lv_obj_t* tabview_bar = lv_tabview_get_tab_bar(tabview);
    lv_obj_set_style_bg_opa(tabview_bar, LV_OPA_20, 0);
    lv_obj_set_style_bg_color(tabview_bar, bar_grey, 0);
}

lv_obj_t* page_manager_tabview_create_tab(lv_obj_t* tabview, const char* tab_name) {
    lv_obj_t* tab = lv_tabview_add_tab(tabview, tab_name);

    lv_obj_t* tab_bar = lv_tabview_get_tab_bar(tabview);
    uint32_t tab_button_index = lv_tabview_get_tab_count(tabview) - 1;

    page_manager_tabview_set_tab_button_styles(lv_obj_get_child(tab_bar, tab_button_index));

    return tab;
}

void page_manager_tabview_set_tab_button_styles(lv_obj_t* tab_btn) {
    lv_obj_set_style_text_color(tab_btn, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(tab_btn, &siemens_sans_global_roman, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(tab_btn, lv_color_white(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(tab_btn, &siemens_sans_global_roman, LV_PART_MAIN | LV_STATE_CHECKED);

    lv_obj_set_style_bg_opa(tab_btn, LV_OPA_50, LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(tab_btn, background_green, LV_STATE_CHECKED);

    lv_obj_set_style_border_color(tab_btn, background_green, LV_STATE_CHECKED);
}

void page_manager_create_pages(lv_obj_t* tabview, uint8_t joystick_vehicle_id, uint8_t joystick_sensor_id) {
    lv_obj_t* vehicle_tab = page_manager_tabview_create_tab(tabview, "Vehicle");
    lv_obj_t* ultrasonic_sensor_tab = page_manager_tabview_create_tab(tabview, "Ultrasonic Sensor");

    page_vehicle_create(vehicle_tab, joystick_vehicle_id);
    page_sensor_create(ultrasonic_sensor_tab, joystick_sensor_id);
}

void page_manager_set_vehicle_page_joystick_cb(joystick_cb_t callback) {
    page_vehicle_set_joystick_cb(callback);
}

void page_manager_set_sensor_page_joystick_cb(joystick_cb_t callback) {
    //page_sensor_set_joystick_cb(callback);
}
