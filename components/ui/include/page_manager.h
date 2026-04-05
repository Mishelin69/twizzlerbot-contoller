#pragma once

#include <misc/lv_types.h>

#include <input/joystick.h>

void page_manager_create(lv_obj_t* container, uint8_t joystick_vehicle_id, uint8_t joystick_sensor_id);

lv_obj_t* page_manager_create_tabview(lv_obj_t* container);

void page_manager_create_pages(lv_obj_t* tabview, uint8_t joystick_vehicle_id, uint8_t joystick_sensor_id);

void page_manager_tabview_set_top_bar_bg(lv_obj_t* tabview);

lv_obj_t* page_manager_tabview_create_tab(lv_obj_t* tabview, const char* tab_name);

void page_manager_tabview_set_tab_button_styles(lv_obj_t* tab_btn);

void page_manager_set_vehicle_page_joystick_cb(joystick_cb_t callback);

void page_manager_set_sensor_page_joystick_cb(joystick_cb_t callback);
