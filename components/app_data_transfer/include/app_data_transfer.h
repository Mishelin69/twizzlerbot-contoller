#pragma once

#include <app_esp_now.h>

void app_data_transfer_hook_callbacks();

void app_data_transfer_on_send_cb(const esp_now_send_info_t* send_info, esp_now_send_status_t status);
