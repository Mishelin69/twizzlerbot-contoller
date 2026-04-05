#include <input/joystick.h>

#include <core/lv_obj_event.h>
#include <core/lv_obj_pos.h>
#include <indev/lv_indev.h>

#include <math.h>
#include <string.h>
#include <styles/styles.h>
#include <styles/colors.h>

void joystick_create(lv_obj_t* parent, joystick_t* joystick, uint32_t radius, joystick_cb_t callback, uint32_t knob_radius, uint8_t id) {
    joystick->callback = callback;
    joystick->radius = radius;
    joystick->knob_radius = knob_radius;
    joystick->max_travel_distance_knob = joystick_calculate_max_travel_distance(radius, knob_radius);
    joystick->id = id; 
    joystick->is_dragging = false;

    joystick_create_ui_elements(joystick, parent);
}

uint32_t joystick_calculate_max_travel_distance(uint32_t radius, uint32_t knob_radius) {
    return radius-knob_radius;
}

void joystick_create_ui_elements(joystick_t* joystick, lv_obj_t* parent) {
    joystick->container = joystick_create_circle_container_and_style_it(joystick, parent);

    joystick_create_second_circle(joystick);
    
    joystick->knob = joystick_create_knob(joystick);

    lv_obj_add_event_cb(joystick->container, joystick_event_cb, LV_EVENT_ALL, joystick);
}

lv_obj_t* joystick_create_circle_container_and_style_it(joystick_t* joystick, lv_obj_t* parent) {
    lv_obj_t* container = style_create_default_object(parent, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(container, bar_grey, LV_STATE_DEFAULT);
    lv_obj_set_size(container, joystick->radius*2, joystick->radius*2);
    lv_obj_set_style_radius(container, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(container, LV_OPA_80, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(container, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(
        container, 
        lv_color_darken(bar_grey, LV_OPA_40),
        LV_STATE_DEFAULT
    );
    lv_obj_remove_flag(
        container, 
        LV_OBJ_FLAG_SCROLLABLE | 
        LV_OBJ_FLAG_SCROLL_CHAIN | 
        LV_OBJ_FLAG_GESTURE_BUBBLE
    );
    return container;
}

void joystick_create_second_circle(joystick_t* joystick) {
    lv_obj_t* container = style_create_default_object(joystick->container, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(container, lv_color_mix(lv_color_white(), bar_grey, LV_OPA_10), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(container, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(container, LV_OPA_80, LV_STATE_DEFAULT);
    lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(
        container, 
        joystick->radius*2*0.6, 
        joystick->radius*2*0.6
    );
    lv_obj_remove_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(container, LV_OBJ_FLAG_EVENT_BUBBLE);
}

lv_obj_t* joystick_create_knob(joystick_t* joystick) {
    lv_obj_t* container = style_create_default_object(joystick->container, LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(container, lv_color_mix(lv_color_white(), background_green, LV_OPA_TRANSP), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(container, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(container, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(container, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(
        container, 
        lv_color_darken(background_green, LV_OPA_40),
        LV_STATE_DEFAULT
    );
    lv_obj_set_size(
        container, 
        joystick->knob_radius*2, 
        joystick->knob_radius*2
    );

    lv_obj_add_flag(container, LV_OBJ_FLAG_EVENT_BUBBLE);

    return container;
}

void joystick_event_cb(lv_event_t* event) {
    joystick_t* joystick = lv_event_get_user_data(event);
    lv_obj_t* container = joystick->container;

    if (!container || !joystick) {
        return;
    }

    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_PRESSED) {
       joystick->is_dragging = joystick_event_cb_has_touch_landed_on_knob(container, event, joystick); 
    }
    else if (joystick->is_dragging && code == LV_EVENT_PRESSING) {
        joystick_event_cb_handle_press(container, event, joystick);
    }
    else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {
        joystick_event_cb_handle_release(joystick);        
    }
}

bool joystick_event_cb_has_touch_landed_on_knob(lv_obj_t* container, lv_event_t* event, joystick_t* joystick) {
    joystick_stick_moved_t initial_touch = joystick_event_cb_get_traveled_distance(container, event, joystick->max_travel_distance_knob);
    float dist_from_center = sqrtf(
        initial_touch.actual_offset_x * initial_touch.actual_offset_x + 
        initial_touch.actual_offset_y * initial_touch.actual_offset_y
    );

    return dist_from_center <= (joystick->knob_radius * 1.5);
}

void joystick_event_cb_handle_release(joystick_t* joystick) {
    joystick->is_dragging = false;
    lv_obj_align(joystick->knob, LV_ALIGN_CENTER, 0, 0);
    joystick_event_cb_send_app_position_update(joystick, 0, 0);
}

void joystick_event_cb_handle_press(lv_obj_t* container, lv_event_t* event, joystick_t* joystick) {
    lv_indev_t* input_device = lv_event_get_indev(event);
    if (!input_device) {
        return;
    }

    joystick_stick_moved_t joystick_moved = joystick_event_cb_get_traveled_distance(container, event, joystick->max_travel_distance_knob);
    if (!joystick_event_cb_was_movement_significant_enough(joystick_moved)) {
        return;
    }

    lv_obj_align(joystick->knob, LV_ALIGN_CENTER, joystick_moved.actual_offset_x, joystick_moved.actual_offset_y);
    joystick_event_cb_send_app_position_update(joystick, joystick_moved.ratio_x, joystick_moved.ratio_y);
}

bool joystick_event_cb_was_movement_significant_enough(joystick_stick_moved_t joystick_movement) {
    if (fabsf(joystick_movement.ratio_x) < 0.01 && fabsf(joystick_movement.ratio_y) < 0.01) {
        return false;
    }
    return true;
}

void joystick_event_cb_send_app_position_update(joystick_t* joystick, float x_val, float y_val) {
    if (!joystick->callback) {
        return;
    }

    (joystick->callback)(joystick->id, x_val, y_val);
}

joystick_stick_moved_t joystick_event_cb_get_traveled_distance(lv_obj_t* container, lv_event_t* event, uint32_t max_travel) {
    lv_indev_t* indev = lv_event_get_indev(event);
    if (!indev) {
        joystick_stick_moved_t ret;
        memset(&ret, 0, sizeof(ret));
        return ret;
    }

    joystick_container_center_t container_center = joystick_event_cb_get_pressed_container_center(container);
    lv_point_t point;
    lv_indev_get_point(indev, &point);

    return joystick_event_cb_scale_delta_values(
            max_travel,
            point.x - container_center.center_x,
            point.y - container_center.center_y
            );
}

joystick_container_center_t joystick_event_cb_get_pressed_container_center(lv_obj_t* container) {
    lv_area_t container_pos;
    lv_obj_get_coords(container, &container_pos);

    joystick_container_center_t ret = {
        .center_x = (container_pos.x1 + container_pos.x2) / 2,
        .center_y  = (container_pos.y1 + container_pos.y2) / 2
    };

    return ret;
}

joystick_stick_moved_t joystick_event_cb_scale_delta_values(uint32_t max_travel, int32_t delta_x, int32_t delta_y) {
    float traveled_distance = sqrtf(delta_x*delta_x + delta_y*delta_y);

    if (traveled_distance > max_travel) {
        float angle_between = atan2(delta_y, delta_x);
        delta_x = (int32_t)(max_travel * cos(angle_between));
        delta_y = (int32_t)(max_travel * sin(angle_between));
    }

    float moved_x_ratio = (float)delta_x / max_travel;
    float moved_y_ratio = -((float)delta_y / max_travel);

    joystick_stick_moved_t ret = {
        .ratio_x = moved_x_ratio,
        .ratio_y = moved_y_ratio,
        .actual_offset_x = delta_x,
        .actual_offset_y = delta_y
    };

    return ret;
}
