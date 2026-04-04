#include <styles/images.h>

#include "esp_log.h"

#define TAG "app_ui_image"

lv_obj_t* create_image(lv_obj_t* parent, const void* src, const char* img_name) {
    if (!src) {
        ESP_LOGE(TAG, "Src was null");
    }

    const lv_image_dsc_t* img_dsc = (const lv_image_dsc_t*)src;

    if (img_dsc->data == NULL) {
        ESP_LOGE(TAG, "Image descriptor data is NULL! Width: %d, Height: %d", 
                 img_dsc->header.w, img_dsc->header.h);
        return NULL;
    }
    
    ESP_LOGI(TAG, "Creating image(%s) - Width: %d, Height: %d, Data: %p", 
             img_name, img_dsc->header.w, img_dsc->header.h, img_dsc->data);
    
    lv_obj_t* img = lv_image_create(parent);
    lv_image_set_src(img, src);
    
    return img;
}

lv_obj_t* create_alien_transparent(lv_obj_t* parent) {
    return create_image(parent, &alien_transparent_background, "settings");
}
