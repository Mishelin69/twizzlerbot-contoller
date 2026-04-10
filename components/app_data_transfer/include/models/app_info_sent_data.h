#pragma once

#include <app_send_data_model.h>
#include <stdint.h>

#define APP_RESEND_DATA_NUM_RETRIES (5)

typedef struct {
    bool succeded;
    app_send_data_info_t data_to_resend;
    uint8_t retries_left;
} app_info_sent_data_t;
