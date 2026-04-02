#include <app_nvs.h>
#include <app.h>

#include <connect_to_wifi.h>

#include <waveshare_rgb_lcd_port.h>

void ram_monitor_task(void* params) {
    while (true) {
        size_t free_ram = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
        size_t free_psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);

        ESP_LOGI(TAG, "Free RAM: %d bytes", free_ram);
        ESP_LOGI(TAG, "Free PSRAM: %d bytes", free_psram);
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void app_main() {
    waveshare_esp32_s3_rgb_lcd_init();
    app_nvs_init();

    ESP_LOGI(TAG, "Starting App");

    // Lock the mutex due to the LVGL APIs not being thread-safe
    if (lvgl_port_lock(-1)) {
        app_create();
        // Release the mutex
        lvgl_port_unlock();
    } 

    ESP_ERROR_CHECK(wifi_init());

    xTaskCreatePinnedToCore(
        ram_monitor_task, 
        "memory monitor", 
        4096,
        NULL,
        5,
        NULL, 
        0
    );
}
