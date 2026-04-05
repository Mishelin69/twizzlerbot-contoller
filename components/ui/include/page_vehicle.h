#pragma once

#include <misc/lv_types.h>

#include <input/joystick.h>

void page_vehicle_create(lv_obj_t* container, uint8_t joystick_id);

void page_vehicle_set_joystick_cb(joystick_cb_t callback);
