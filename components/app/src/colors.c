#include <colors.h>

lv_color_t orange_color = LV_COLOR_MAKE(0xec, 0x66, 0x02);
lv_color_t white_color = LV_COLOR_MAKE(0xff, 0xff, 0xff);
lv_color_t bar_grey = LV_COLOR_MAKE(0x14, 0x14, 0x14);
lv_color_t background_grey = LV_COLOR_MAKE(0x28, 0x28, 0x28);
lv_color_t background_light_grey = LV_COLOR_MAKE(0x3c, 0x3c, 0x3c);
lv_color_t background_red = LV_COLOR_MAKE(0xe5, 0x36, 0x33);
lv_color_t background_green = LV_COLOR_MAKE(0x00, 0x9a, 0x38);

lv_style_t style_text_white;
lv_style_t style_text_orange;
lv_style_t style_bar_grey;
lv_style_t style_background_grey;
lv_style_t style_background_light_grey;
lv_style_t style_background_red;
lv_style_t style_background_green;

void initialize_color_styles() {
    lv_style_init(&style_text_white);
    lv_style_set_text_color(&style_text_white, white_color);
    lv_style_init(&style_text_orange);
    lv_style_set_text_color(&style_text_orange, orange_color);
    lv_style_init(&style_bar_grey);
    lv_style_set_text_color(&style_bar_grey, bar_grey);
    lv_style_init(&style_background_grey);
    lv_style_set_text_color(&style_background_grey, background_grey);
    lv_style_init(&style_background_red);
    lv_style_set_text_color(&style_background_red, background_red);
    lv_style_init(&style_background_green);
    lv_style_set_text_color(&style_background_green, background_green);
    lv_style_init(&style_background_light_grey);
    lv_style_set_text_color(&style_background_light_grey, background_light_grey);
}
