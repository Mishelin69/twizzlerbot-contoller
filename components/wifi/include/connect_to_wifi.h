#pragma once

#include "esp_wifi.h"
#include "sdkconfig.h"

#define WIFI_AUTHMODE WIFI_AUTH_WPA2_PSK
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#define WIFI_SSID_BUFFER_SIZE (128)
#define WIFI_PASSWORD_BUFFER_SIZE (128)

#define WIFI_CONNECTION_RETRY_TIMEOUT (5000000)

bool wifi_check_if_connection_successful();

esp_err_t wifi_init();

esp_err_t wifi_cleanup();

esp_err_t connect_to_wifi(const char* ssid, const char* password);

esp_err_t wifi_disconnect();

void wifi_connection_task(void *pvParameters); 

void wifi_create_connection_task(); 

char* wifi_try_load_wifi_ssid();

char* wifi_try_load_wifi_password();

void wifi_set_status_connected();

void wifi_set_status_disconnected();

void wifi_reconnect_callback(void* arg);

void wifi_create_reconnect_timer();
