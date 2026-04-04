#pragma once

#include <widgets/image/lv_image.h>

LV_IMAGE_DECLARE(alien_transparent_background);

typedef struct _lv_obj_t lv_obj_t;

lv_obj_t* create_image(lv_obj_t* parent, const void* src, const char* img_name);

lv_obj_t* create_alien_transparent(lv_obj_t* parent);
