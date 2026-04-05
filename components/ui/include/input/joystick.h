#pragma once

#include <misc/lv_types.h>
#include <stdint.h>
#include <stdbool.h>

typedef void (*joystick_cb_t)(uint8_t id, float x_val, float y_val);

typedef struct {
    uint8_t id;

    lv_obj_t* knob;
    lv_obj_t* container;

    bool is_dragging;

    uint32_t radius;
    uint32_t knob_radius;
    uint32_t max_travel_distance_knob;

    joystick_cb_t callback;
} joystick_t;

typedef struct {
    float ratio_x;
    float ratio_y;
    int32_t actual_offset_x;
    int32_t actual_offset_y;
} joystick_stick_moved_t;

typedef struct {
    int32_t center_x;
    int32_t center_y;
} joystick_container_center_t;

void joystick_create(lv_obj_t* parent, joystick_t* joystick, uint32_t radius, joystick_cb_t callback, uint32_t knob_radius, uint8_t id);

void joystick_create_ui_elements(joystick_t* joystick, lv_obj_t* parent);

uint32_t joystick_calculate_max_travel_distance(uint32_t radius, uint32_t knob_radius);

lv_obj_t* joystick_create_circle_container_and_style_it(joystick_t* joystick, lv_obj_t* parent);

void joystick_create_second_circle(joystick_t* joystick);

lv_obj_t* joystick_create_knob(joystick_t* joystick);

void joystick_event_cb(lv_event_t* event);

bool joystick_event_cb_has_touch_landed_on_knob(lv_obj_t* container, lv_event_t* event, joystick_t* joystick);

void joystick_event_cb_handle_release(joystick_t* joystick);

void joystick_event_cb_handle_press(lv_obj_t* container, lv_event_t* event, joystick_t* joystick);

void joystick_event_cb_send_app_position_update(joystick_t* joystick, float x_val, float y_val);

joystick_stick_moved_t joystick_event_cb_get_traveled_distance(lv_obj_t* container, lv_event_t* event, uint32_t max_travel);

joystick_container_center_t joystick_event_cb_get_pressed_container_center(lv_obj_t* container);

joystick_stick_moved_t joystick_event_cb_scale_delta_values(uint32_t max_travel, int32_t delta_x, int32_t delta_y);

bool joystick_event_cb_was_movement_significant_enough(joystick_stick_moved_t joystick_movement);
