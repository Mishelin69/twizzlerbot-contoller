#include <styles/text.h>

#include <misc/lv_area.h>
#include <widgets/label/lv_label.h>

void initialize_text_styles() {
    lv_style_init(&text_style_bold);
    lv_style_set_text_font(&text_style_bold, &siemens_sans_global_bold);
    lv_style_init(&text_style_roman);
    lv_style_set_text_font(&text_style_roman, &siemens_sans_global_roman);
    lv_style_init(&text_style_big);
    lv_style_set_text_font(&text_style_big, &siemens_sans_bold_big);
    lv_style_init(&text_style_next_reservation_text);
    lv_style_set_text_font(&text_style_next_reservation_text, &siemens_sans_next_reservation_text);
    lv_style_init(&text_style_registration_text);
    lv_style_set_text_font(&text_style_registration_text, &siemens_sans_registration_text);
}

lv_obj_t* create_text_label_roman(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector) {
    if (!parent || !text) {
        return NULL;
    }

    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_add_style(label, &text_style_roman, selector);
    apply_color_to_text(label, text_color, selector);

    return label;
}

lv_obj_t* create_text_label_roman_with_overflow(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector) {
    if (!parent || !text) {
        return NULL;
    }

    lv_obj_t* label = create_text_label_roman(parent, text, text_color, selector);
    text_set_overflow_behaviour(label);

    return label;
}

lv_obj_t* create_text_label_bold(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector) {
    if (!parent || !text) {
        return NULL;
    }

    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_add_style(label, &text_style_bold, selector);
    apply_color_to_text(label, text_color, selector);

    return label;
}

lv_obj_t* create_text_label_big(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector) {
    if (!parent || !text) {
        return NULL;
    }

    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_add_style(label, &text_style_big, selector);
    apply_color_to_text(label, text_color, selector);

    return label;

}

lv_obj_t* create_text_label_next_reservation(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector) {
    if (!parent || !text) {
        return NULL;
    }

    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_add_style(label, &text_style_next_reservation_text, selector);
    apply_color_to_text(label, text_color, selector);

    return label;
}

lv_obj_t* create_text_registration(lv_obj_t* parent, const char* text, TextColor text_color, lv_style_selector_t selector) {
    if (!parent || !text) {
        return NULL;
    }

    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_add_style(label, &text_style_registration_text, selector);
    apply_color_to_text(label, text_color, selector);

    return label;
}

void apply_color_to_text(lv_obj_t* object, TextColor text_color, lv_style_selector_t selector) {
    if (!object) {
        return;
    }

    switch (text_color) {
        case TEXT_COLOR_WHITE:
            lv_obj_add_style(object, &style_text_white, selector);
            break;
        case TEXT_COLOR_ORANGE:
            lv_obj_add_style(object, &style_text_orange, selector);
            break;
        case TEXT_COLOR_GREEN:
            lv_obj_add_style(object, &style_text_green, selector);
            break;
        default:
            break;
    }
}

void text_set_overflow_behaviour(lv_obj_t* object) {
    if (!object) {
        return;
    }

    lv_obj_set_width(object, lv_pct(100));
    lv_obj_set_height(object, 24);
    lv_label_set_long_mode(object, LV_LABEL_LONG_MODE_DOTS);
}
