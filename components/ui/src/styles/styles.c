#include <input/button.h>

#include <styles/styles.h>
#include <styles/images.h>
#include <styles/colors.h>
#include <styles/text.h>

lv_obj_t* style_create_default_object(lv_obj_t* parent, lv_style_selector_t selector) {
    if (!parent) {
        return NULL;
    }

    lv_obj_t* obj = lv_obj_create(parent);
    lv_obj_set_style_margin_all(obj, 0, selector);
    lv_obj_set_style_pad_all(obj, 0, selector);
    lv_obj_set_style_border_width(obj, 0, selector);
    lv_obj_set_style_radius(obj, 0, selector);
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, selector);
    lv_obj_set_style_pad_gap(obj, 10, selector);

    return obj;
}

lv_obj_t* style_create_flex_vertical_container(lv_obj_t* parent, lv_style_selector_t selector) {
    lv_obj_t* container = style_create_default_object(parent, selector);

    lv_obj_set_align(container, LV_ALIGN_CENTER);
    lv_obj_set_layout(container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    return container;
}

lv_obj_t* style_create_flex_vertical_list(lv_obj_t* parent, lv_style_selector_t selector) {
    lv_obj_t* container = style_create_default_object(parent, selector);

    lv_obj_set_align(container, LV_ALIGN_TOP_LEFT);
    lv_obj_set_layout(container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    return container;
}

void style_create_clickable_with_image_and_text(lv_obj_t* button, const char* text, const void* img_src) {
    if (!button || !text || !img_src) {
        return;
    }

    lv_obj_t* container = style_create_grid_vertical_container(button, LV_STATE_DEFAULT);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_remove_flag(container, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* image = create_image(container, img_src, "clickable_with_image");
    lv_obj_set_grid_cell(image, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_END, 0, 1);
    
    lv_obj_t* text_label = create_text_label_bold(container, text, TEXT_COLOR_WHITE, LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(text_label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
}

lv_obj_t* style_create_grid_vertical_container(lv_obj_t* parent, lv_style_selector_t selector) {
    if (!parent) {
        return NULL;
    }

    lv_obj_t* container = style_create_default_object(parent, selector);

    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_FR(63), LV_GRID_FR(37), LV_GRID_TEMPLATE_LAST};
    
    lv_obj_set_layout(container, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(container, col_dsc, row_dsc);
    lv_obj_set_align(container, LV_ALIGN_CENTER);
    
    return container;
}
