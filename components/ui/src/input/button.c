#include "input/button.h"

#include "styles/styles.h"
#include "styles/colors.h"

lv_obj_t* style_create_default_button(lv_obj_t* parent, lv_style_selector_t selector) {
    lv_obj_t* btn = lv_button_create(parent);
    lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, selector);
    lv_obj_set_style_border_width(btn, 0, selector);
    lv_obj_set_style_outline_width(btn, 0, selector);
    lv_obj_set_style_shadow_width(btn, 0, selector);
    lv_obj_set_style_radius(btn, 0, selector);
    lv_obj_set_style_pad_all(btn, 0, selector);
    lv_obj_set_style_margin_all(btn, 0, selector);

    return btn;
}

lv_obj_t* style_create_single_button_container(lv_obj_t* parent, ButtonContainerSize size) {
    if (!parent) {
        return NULL;
    }

    lv_obj_t* button_box = style_create_default_object(parent, LV_STATE_DEFAULT);

    if (size == BUTTON_CONTAINER_SIZE_BIG) {
        lv_obj_set_size(button_box, BIG_BUTTON_WIDTH, BIG_BUTTON_HEIGHT);
    } 
    else {
        lv_obj_set_size(button_box, BUTTON_WIDTH, BUTTON_CONTAINER_HEIGHT);
    }

    lv_obj_set_style_bg_color(button_box, background_light_grey, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(button_box, LV_OPA_COVER, LV_STATE_DEFAULT);

    return button_box;
}

lv_obj_t* style_create_buttons_container(lv_obj_t* parent, ButtonContainerSize size) {
    if (!parent) {
        return NULL;
    }

    lv_obj_t* container = style_create_default_object(parent, LV_STATE_DEFAULT);
    style_button_container_set_styles(container, size);

    return container;
}

void style_button_container_set_styles(lv_obj_t* container, ButtonContainerSize size) {
    lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_layout(container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_obj_set_style_bg_color(container, background_red, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_gap(container, 0, LV_STATE_DEFAULT);

    if (size == BUTTON_CONTAINER_SIZE_BIG) {
        lv_obj_set_size(container, BUTTON_CONTAINER_WIDTH, BIG_BUTTON_HEIGHT);
    } 
    else {
        lv_obj_set_size(container, BUTTON_CONTAINER_WIDTH, BUTTON_CONTAINER_HEIGHT);
    }
}

lv_obj_t* style_create_small_buttons_container(lv_obj_t* parent) {
    if (!parent) {
        return NULL;
    }

    return style_create_buttons_container(parent, BUTTON_CONTAINER_SIZE_SMALL);
}

lv_obj_t* style_create_big_buttons_container(lv_obj_t* parent) {
    if (!parent) {
        return NULL;
    }

    return style_create_buttons_container(parent, BUTTON_CONTAINER_SIZE_BIG);
}

lv_obj_t* style_create_single_small_button_container(lv_obj_t* parent) {
    if (!parent) {
        return NULL;
    }

    return style_create_single_button_container(parent, BUTTON_CONTAINER_SIZE_SMALL);
}

lv_obj_t* style_create_single_big_button_container(lv_obj_t* parent) {
    if (!parent) {
        return NULL;
    }

    return style_create_single_button_container(parent, BUTTON_CONTAINER_SIZE_BIG);
}
