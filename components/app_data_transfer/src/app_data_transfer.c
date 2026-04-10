#include <app_data_transfer.h>

#include <app_queue_send_data.h>

#include <esp_log.h>

#define TAG "app_data_transfer.c"

void app_data_transfer_on_send_cb(const esp_now_send_info_t* send_info, esp_now_send_status_t status) {
    app_send_data_send_ack_signal();
    //ESP_LOGI(TAG, "On send callback");
}

void app_data_transfer_hook_callbacks() {
    app_espnow_attach_on_send_cb(app_data_transfer_on_send_cb);
}
