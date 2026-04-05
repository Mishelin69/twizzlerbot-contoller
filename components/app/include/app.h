#pragma once

#include <stdint.h>

void app_create();

void app_initialize_ui();

void app_set_callbacks();

void app_joystick_cb(uint8_t, float x_val, float y_val);
