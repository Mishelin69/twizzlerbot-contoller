#pragma once

#include <esp_err.h>
#include <esp_now.h>

typedef void (*espnow_on_send_cb_t)(const esp_now_send_info_t*,esp_now_send_status_t);

extern uint8_t ROBOT_CONTROLLER_MAC[6];

esp_err_t app_espnow_init();

esp_err_t app_espnow_add_peer(const uint8_t peer_mac[6]);

esp_err_t app_espnow_send(const uint8_t mac_addr[6], uint8_t* data, uint32_t data_len);

void app_espnow_attach_on_send_cb(espnow_on_send_cb_t callback);
