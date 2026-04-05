#include <page_vehicle.h>

#include <input/joystick.h>

static joystick_t JOYSTICK;

void page_vehicle_create(lv_obj_t* container, uint8_t joystick_id) {
    joystick_create(container, &JOYSTICK, 150, NULL, 40, joystick_id);
}

void page_vehicle_set_joystick_cb(joystick_cb_t callback) {
    JOYSTICK.callback = callback;
}
