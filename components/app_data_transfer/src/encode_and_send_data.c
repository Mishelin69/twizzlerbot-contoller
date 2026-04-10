#include "app_esp_now.h"
#include <encode_and_send_data.h>

#include <nanopb/pb.h>
#include <nanopb/pb_encode.h>

#include <protodefs/MotorData.pb.h>
#include <stdint.h>

#include <esp_log.h>

#define TAG "encode_and_send_data.c"

void app_encode_and_send_data(app_send_data_info_t data_to_send) {
    uint8_t data_buffer[Request_size];
    Request new_request = { 
        .command = data_to_send.vehicle_type,
        .id = 0,
        .x_movement = data_to_send.dir_x,
        .y_movement = data_to_send.dir_y
    };

    pb_ostream_t stream = pb_ostream_from_buffer(data_buffer, sizeof(data_buffer));
    bool status = pb_encode(&stream, Request_fields, &new_request);

    if (!status) {
        ESP_LOGE(TAG, "Failed to encode protobuf message: %s", PB_GET_ERROR(&stream));
    }

    app_espnow_send(data_to_send.mac, data_buffer, stream.bytes_written);
}
