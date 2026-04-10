#include "connect_to_wifi.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_event.h"

#include "esp_wifi.h"
#include "freertos/event_groups.h"

#include <inttypes.h>
#include <esp_now.h>

#define TAG "wifi_connect"

esp_err_t wifi_init() { 
    esp_err_t ret = esp_netif_init();

    if (ret != ESP_OK) {
        return ret;
    }

    ret = esp_event_loop_create_default();

    if (ret != ESP_OK) {
        return ret;
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ret = esp_wifi_init(&cfg);

    if (ret != ESP_OK) {
        return ret;
    }

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_channel(APP_WIFI_ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE));

    return ESP_OK;
}
