#include "connect_to_wifi.h"

#include "app_nvs.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_event.h"

#include "esp_timer.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include <inttypes.h>
#include <string.h>

#define TAG "wifi_connect"

static char WIFI_SSID_BUFFER[WIFI_SSID_BUFFER_SIZE];
static char WIFI_PASS_BUFFER[WIFI_PASSWORD_BUFFER_SIZE];

static esp_netif_t* netif = NULL;
static esp_event_handler_instance_t ip_event_handler;
static esp_event_handler_instance_t wifi_event_handler;

static EventGroupHandle_t s_wifi_event_group = NULL;

static esp_timer_handle_t WIFI_RECONNECT_TIMER;

static void ip_event_cb(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    ESP_LOGI(TAG, "Handling IP event, event code 0x%" PRIx32, event_id);

    switch (event_id) {
        case (IP_EVENT_STA_GOT_IP):
            ip_event_got_ip_t* event_ip = (ip_event_got_ip_t *)event_data;
            ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event_ip->ip_info.ip));
            wifi_set_status_connected();
            break;
        case (IP_EVENT_STA_LOST_IP):
            wifi_set_status_disconnected();
            ESP_LOGE(TAG, "Lost IP");
            break;
        case (IP_EVENT_GOT_IP6):
            ip_event_got_ip6_t* event_ip6 = (ip_event_got_ip6_t *)event_data;
            ESP_LOGI(TAG, "Got IPv6: " IPV6STR, IPV62STR(event_ip6->ip6_info.ip));
            wifi_set_status_connected();
            break;
        default:
            ESP_LOGI(TAG, "IP event not handled");
            break;
    }
}

void wifi_set_status_connected() {
    esp_timer_stop(WIFI_RECONNECT_TIMER);
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
}

void wifi_set_status_disconnected() {
    esp_timer_stop(WIFI_RECONNECT_TIMER);
    esp_timer_start_once(WIFI_RECONNECT_TIMER, WIFI_CONNECTION_RETRY_TIMEOUT);
    xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
}

void wifi_reconnect_callback(void* arg) {
    ESP_LOGI(TAG, "Retrying to connect to Wi-Fi network...");
    xEventGroupClearBits(s_wifi_event_group, WIFI_FAIL_BIT);
    esp_wifi_connect();
}

static void wifi_event_cb(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    ESP_LOGI(TAG, "Handling Wi-Fi event, event code 0x%" PRIx32, event_id);

    switch (event_id) {
        case (WIFI_EVENT_WIFI_READY):
            ESP_LOGI(TAG, "Wi-Fi ready");
            break;
        case (WIFI_EVENT_SCAN_DONE):
            ESP_LOGI(TAG, "Wi-Fi scan done");
            break;
        case (WIFI_EVENT_STA_START):
            ESP_LOGI(TAG, "Wi-Fi started, connecting to AP...");
            esp_wifi_connect();
            break;
        case (WIFI_EVENT_STA_STOP):
            ESP_LOGI(TAG, "Wi-Fi stopped");
            break;
        case (WIFI_EVENT_STA_CONNECTED):
            ESP_LOGI(TAG, "Wi-Fi connected");
            break;
        case (WIFI_EVENT_STA_DISCONNECTED):
            ESP_LOGI(TAG, "Wi-Fi disconnected");
            wifi_set_status_disconnected();
            break;
        case (WIFI_EVENT_STA_AUTHMODE_CHANGE):
            ESP_LOGI(TAG, "Wi-Fi authmode changed");
            break;
        default:
            ESP_LOGI(TAG, "Wi-Fi event not handled");
            break;
    }
}

esp_err_t wifi_init() { 
    s_wifi_event_group = xEventGroupCreate();
    esp_err_t ret = esp_netif_init();

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize TCP/IP network stack");
        return ret;
    }

    ret = esp_event_loop_create_default();

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create default event loop");
        return ret;
    }

    ret = esp_wifi_set_default_wifi_sta_handlers();

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set default handlers");
        return ret;
    }

    netif = esp_netif_create_default_wifi_sta();

    if (netif == NULL) {
        ESP_LOGE(TAG, "Failed to create default WiFi STA interface");
        return ESP_FAIL;
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &wifi_event_cb,
        NULL,
        &wifi_event_handler
    ));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        ESP_EVENT_ANY_ID,
        &ip_event_cb,
        NULL,
        &ip_event_handler
    ));

    wifi_create_reconnect_timer();

    return ret;
}

void wifi_create_reconnect_timer() {
    const esp_timer_create_args_t wifi_reconnect_timer_args = {
        .callback = &wifi_reconnect_callback,
        .name = "wifi-reconnect"
    };

    ESP_ERROR_CHECK(esp_timer_create(&wifi_reconnect_timer_args, &WIFI_RECONNECT_TIMER));
}

esp_err_t wifi_cleanup() {
    esp_err_t ret = esp_wifi_stop();

    if (ret == ESP_ERR_WIFI_NOT_INIT) {
        ESP_LOGE(TAG, "Wi-Fi stack not initialized");
        return ret;
    }

    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(netif));
    esp_netif_destroy(netif);

    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, ESP_EVENT_ANY_ID, ip_event_handler));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler));

    return ESP_OK;
}

esp_err_t connect_to_wifi(const char* ssid, const char* password) {
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTHMODE,
        },
    };

    strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    ESP_LOGI(TAG, "Connecting to Wi-Fi network: %s", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(
        s_wifi_event_group, 
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE, pdFALSE, portMAX_DELAY
    );

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to Wi-Fi network: %s", wifi_config.sta.ssid);
        return ESP_OK;
    } 
    else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGE(TAG, "Failed to connect to Wi-Fi network: %s", wifi_config.sta.ssid);
        return ESP_FAIL;
    }

    ESP_LOGE(TAG, "Unexpected Wi-Fi error");

    return ESP_FAIL;
}

bool wifi_check_if_connection_successful() {
    if (!s_wifi_event_group) {
        return false;
    }

    EventBits_t bits = xEventGroupGetBits(s_wifi_event_group);

    if (bits & WIFI_FAIL_BIT || !(bits & WIFI_CONNECTED_BIT)) {
        return false;
    }

    wifi_ap_record_t ap_info;
    esp_netif_ip_info_t ip_info;

    if (esp_wifi_sta_get_ap_info(&ap_info) != ESP_OK) {
        return false; 
    }

    esp_netif_t* sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

    if (!sta_netif || esp_netif_get_ip_info(sta_netif, &ip_info) != ESP_OK) {
        return false;        
    }

    if (ip_info.ip.addr == 0 || ip_info.gw.addr == 0) {
        return false;
    }

    return true; 
}

esp_err_t wifi_disconnect(void) {
    if (s_wifi_event_group) {
        vEventGroupDelete(s_wifi_event_group);
    }

    return esp_wifi_disconnect();
}

void wifi_connection_task(void *pvParameters) {
    ESP_LOGI(TAG, "Starting wifi...");
    ESP_ERROR_CHECK(wifi_init());

    char* wifi_ssid = wifi_try_load_wifi_ssid();
    char* wifi_pass = wifi_try_load_wifi_password();

    esp_err_t ret = connect_to_wifi(wifi_ssid, wifi_pass);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "WiFi connection successful!");
    } 
    else {
        ESP_LOGE(TAG, "WiFi connection failed!");
    }

    vTaskDelete(NULL);
}

char* wifi_try_load_wifi_ssid() {
    app_nvs_return_t status = app_nvs_read_str("wifi_config", "wifi_ssid", WIFI_SSID_BUFFER, WIFI_SSID_BUFFER_SIZE);

    if (status == APP_NVS_NOT_FOUND) {
        app_nvs_write_str("wifi_config", "wifi_ssid", CONFIG_WIFI_SSID);
    }

    return WIFI_SSID_BUFFER;
}

char* wifi_try_load_wifi_password() {
    app_nvs_return_t status = app_nvs_read_str("wifi_config", "wifi_password", WIFI_PASS_BUFFER, WIFI_PASSWORD_BUFFER_SIZE);

    if (status == APP_NVS_NOT_FOUND) {
        app_nvs_write_str("wifi_config", "wifi_password", CONFIG_WIFI_PASSWORD);
    }

    return WIFI_PASS_BUFFER;
}

void wifi_create_connection_task() {
    xTaskCreatePinnedToCore(
        wifi_connection_task,
        "wifi_connect", 
        8192,
        NULL,
        6,
        NULL, 
        0
    );
}
