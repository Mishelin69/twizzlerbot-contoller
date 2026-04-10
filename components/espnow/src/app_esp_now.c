#include "esp_now.h"
#include <app_esp_now.h>

#include <esp_log.h>
#include <string.h>

#include <esp_mac.h>

#include <connect_to_wifi.h>

#define TAG "app_esp_now.c"

static bool ESPNOW_HAS_BEEN_INIT = false;
static bool ESPNOW_ON_SEND_CB_SET = false;

uint8_t ROBOT_CONTROLLER_MAC[6] = {0x08, 0xA6, 0xF7, 0x6A, 0x1E, 0x54};

esp_err_t app_espnow_init() {

    ESP_LOGI(TAG, "Trying to initialize ESP-Now");

    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(app_espnow_add_peer(ROBOT_CONTROLLER_MAC));

    ESP_LOGI(TAG, "Initialized ESP-Now successfully");
    ESPNOW_HAS_BEEN_INIT = true;

    return ESP_OK; 
}

void app_espnow_attach_on_send_cb(espnow_on_send_cb_t callback) {
    if (!callback) {
        ESP_LOGE(TAG, "Func pointer was null!");
        return;
    }

    ESP_ERROR_CHECK(esp_now_register_send_cb(callback));
    ESPNOW_ON_SEND_CB_SET = true;
}

esp_err_t app_espnow_add_peer(const uint8_t peer_mac[6]) {
    esp_now_peer_info_t peer_info = {};

    memcpy(peer_info.peer_addr, peer_mac, 6*sizeof(char));
    peer_info.channel = APP_WIFI_ESPNOW_CHANNEL;
    peer_info.ifidx = WIFI_IF_STA;
    peer_info.encrypt = false;

    esp_err_t add_resp_status = esp_now_add_peer(&peer_info);

    if (add_resp_status == ESP_OK) {
        ESP_LOGI(TAG, "Added peer " MACSTR " successfully", MAC2STR(peer_mac));
        return add_resp_status;
    }
    else {
        ESP_LOGE(TAG, "Adding peer " MACSTR " failed. (error:%s)", MAC2STR(peer_mac), esp_err_to_name(add_resp_status));
        return ESP_OK;
    }
}

esp_err_t app_espnow_send(const uint8_t mac_addr[6], uint8_t* data, uint32_t data_len) {
    if (!data || data_len < 1) {
        return ESP_ERR_INVALID_ARG;
    }
    
    if (!ESPNOW_HAS_BEEN_INIT || !esp_now_is_peer_exist(mac_addr) || !ESPNOW_ON_SEND_CB_SET) {
        ESP_LOGE(TAG, "Peer hasn't been added or ESP-Now hasn't been initialized!");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = esp_now_send(mac_addr, data, data_len);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Successfully sent data to: " MACSTR, MAC2STR(mac_addr));
    }
    else {
        ESP_LOGE(TAG, "Failed sending data to: " MACSTR " (error: %s)", MAC2STR(mac_addr), esp_err_to_name(ret));
    }

    return ret;
}
