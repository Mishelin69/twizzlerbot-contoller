#include <app_queue_send_data.h>

#include <enums/command_vehicle_type_enum.h>
#include <app.h>
#include <app_ui.h>
#include <app_joystick_id_defs.h>

#include <app_data_transfer.h>

#include <styles/text.h>
#include <styles/colors.h>

#include <page_manager.h>

#include <esp_log.h>

#define TAG "app.c"

void app_create() {
    app_initialize_ui();
    app_ui_create_ui_elements();
    app_set_callbacks();
    app_create_send_data_queue();
    app_create_send_data_task();
    app_data_transfer_hook_callbacks();
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
    command_vehicle_type_t vehicle_type = COMMAND_UNKNOWN;

    switch (id) {
        case APP_JOYSTICK_VEHICLE_ID:
            vehicle_type = COMMAND_VEHICLE;
            break;
        case APP_JOYSTICK_SENSOR_ID:
            vehicle_type = COMMAND_SERVO;
            break;
    }

    app_send_data_enqueue(vehicle_type, y_val, x_val);
}
