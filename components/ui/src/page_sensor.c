#include <page_sensor.h>

#include <styles/text.h>

void page_sensor_create(lv_obj_t* container, uint8_t joystick_id) {
    create_text_label_roman(container, "Sensor page", TEXT_COLOR_WHITE, 0);
}
