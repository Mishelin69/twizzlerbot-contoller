#pragma once

#include <enums/command_vehicle_type_enum.h>
#include <stdint.h>

typedef struct {
    uint8_t* mac;
    command_vehicle_type_t vehicle_type;
    float dir_y;
    float dir_x;
} app_send_data_info_t;
