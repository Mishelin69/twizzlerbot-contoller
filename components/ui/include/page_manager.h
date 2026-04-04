#pragma once

#include <misc/lv_types.h>

void page_manager_create(lv_obj_t* container);

lv_obj_t* page_manager_create_tabview(lv_obj_t* container);

void page_master_create_pages(lv_obj_t* tabview);

void page_master_tabview_set_top_bar_bg(lv_obj_t* tabview);

lv_obj_t* page_master_tabview_create_tab(lv_obj_t* tabview, const char* tab_name);

void page_master_tabview_set_tab_button_styles(lv_obj_t* tab_btn);
