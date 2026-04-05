#include <app.h>
#include <app_ui.h>
#include <app_joystick_id_defs.h>

#include <styles/text.h>
#include <styles/colors.h>

#include <page_manager.h>

#include <esp_log.h>

#define TAG "app.c"

void app_create() {
    app_initialize_ui();
    app_ui_create_ui_elements();
    app_set_callbacks();
}

void app_initialize_ui() {
    initialize_text_styles();
    initialize_color_styles();
}

void app_set_callbacks() {
    page_manager_set_vehicle_page_joystick_cb(app_joystick_cb);
    page_manager_set_sensor_page_joystick_cb(app_joystick_cb);
}

void app_joystick_cb(uint8_t id, float x_val, float y_val) {
    ESP_LOGI(TAG, "Current position(%d) x: %f ||| y: %f", id, x_val, y_val);
}
