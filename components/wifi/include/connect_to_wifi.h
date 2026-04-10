#pragma once

#define APP_WIFI_ESPNOW_CHANNEL (1)

#include "esp_wifi.h"
#include "sdkconfig.h"

esp_err_t wifi_init();
