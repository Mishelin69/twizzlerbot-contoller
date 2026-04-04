#include "styles/text.h"
#include <page_sensor.h>

void page_sensor_create(lv_obj_t* container) {
    create_text_label_roman(container, "Sensor page", TEXT_COLOR_WHITE, 0);
}
