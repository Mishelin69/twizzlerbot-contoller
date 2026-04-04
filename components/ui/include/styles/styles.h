#pragma once

#include <lvgl.h>

#include "enums/buttons_container_size_enum.h"

lv_obj_t* style_create_default_object(lv_obj_t* parent, lv_style_selector_t selector);

lv_obj_t* style_create_flex_vertical_container(lv_obj_t* parent, lv_style_selector_t selector);

lv_obj_t* style_create_grid_vertical_container(lv_obj_t* parent, lv_style_selector_t selector);

void style_create_clickable_with_image_and_text(lv_obj_t* button, const char* text, const void* img_src);

lv_obj_t* style_create_flex_vertical_list(lv_obj_t* parent, lv_style_selector_t selector);
