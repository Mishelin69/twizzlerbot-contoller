#pragma once

#include <styles/colors.h>
#include <enums/text_color_enum.h>

LV_FONT_DECLARE(siemens_sans_global_roman);
LV_FONT_DECLARE(siemens_sans_global_bold);
LV_FONT_DECLARE(siemens_sans_bold_big);
LV_FONT_DECLARE(siemens_sans_next_reservation_text);
LV_FONT_DECLARE(siemens_sans_registration_text);

static lv_style_t text_style_bold;
static lv_style_t text_style_roman;
static lv_style_t text_style_big;
static lv_style_t text_style_next_reservation_text;
static lv_style_t text_style_registration_text;

void initialize_text_styles();

lv_obj_t* create_text_label_roman(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector);

lv_obj_t* create_text_label_roman_with_overflow(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector);

lv_obj_t* create_text_label_bold(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector);

lv_obj_t* create_text_label_big(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector);

lv_obj_t* create_text_label_next_reservation(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector);

lv_obj_t* create_text_registration(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector);

void apply_color_to_text(lv_obj_t* object, TextColor text_color, lv_style_selector_t selector);

void text_set_overflow_behaviour(lv_obj_t* object);
