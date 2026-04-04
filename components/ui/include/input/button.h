#pragma once

#include "enums/buttons_container_size_enum.h"
#include "lvgl.h"

#define BUTTON_CONTAINER_HEIGHT (145)
#define BUTTON_WIDTH (BUTTON_CONTAINER_HEIGHT)
#define BUTTON_CONTAINER_WIDTH (550)

#define BIG_BUTTON_CONTAINER_HEIGHT (175)
#define BIG_BUTTON_WIDTH (BIG_BUTTON_CONTAINER_HEIGHT-15)
#define BIG_BUTTON_HEIGHT (BIG_BUTTON_CONTAINER_HEIGHT)

lv_obj_t* style_create_default_button(lv_obj_t* parent, lv_style_selector_t selector);

lv_obj_t* style_create_single_button_container(lv_obj_t* parent, ButtonContainerSize size);

lv_obj_t* style_create_buttons_container(lv_obj_t* parent, ButtonContainerSize size);

void style_button_container_set_styles(lv_obj_t* container, ButtonContainerSize size);

lv_obj_t* style_create_small_buttons_container(lv_obj_t* parent);

lv_obj_t* style_create_big_buttons_container(lv_obj_t* parent);

lv_obj_t* style_create_single_small_button_container(lv_obj_t* parent);

lv_obj_t* style_create_single_big_button_container(lv_obj_t* parent);
