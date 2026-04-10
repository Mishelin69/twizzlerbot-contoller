#include "encode_and_send_data.h"
#include <app_queue_send_data.h>

#include <app_esp_now.h>

#include <esp_log.h>

#include <sdkconfig.h>

#define TAG "app_queue_send_data.c"

static QueueHandle_t DATA_TO_SEND_QUEUE = NULL;
static EventGroupHandle_t CAN_SEND_DATA;

void app_create_send_data_queue() {
    ESP_LOGI(TAG, "Create queue for sending data");
    DATA_TO_SEND_QUEUE = xQueueCreate(1, sizeof(app_send_data_info_t));

    if (!DATA_TO_SEND_QUEUE) {
        ESP_LOGE(TAG, "Failed creating queue");
        return;
    }

    ESP_LOGI(TAG, "Created queue successfully :)");

    if (!app_send_data_create_event_group()) {
        ESP_LOGE(TAG, "Failed creating event group");
        vQueueDelete(DATA_TO_SEND_QUEUE);
        DATA_TO_SEND_QUEUE = NULL;
    }
}

bool app_send_data_create_event_group() {
    CAN_SEND_DATA = xEventGroupCreate();

    EventBits_t set_bits = xEventGroupSetBits(CAN_SEND_DATA, APP_CAN_SEND_DATA_BIT);

    if ((set_bits & APP_CAN_SEND_DATA_BIT) == 0) {

        vEventGroupDelete(CAN_SEND_DATA);
        CAN_SEND_DATA = NULL;

        return false; 
    }

    return CAN_SEND_DATA != NULL;
}

void app_send_data_send_ack_signal() {
    EventBits_t bits = xEventGroupGetBits(CAN_SEND_DATA);

    if ((bits & APP_WAITING_FOR_ACK) != 0) {
        xEventGroupClearBits(CAN_SEND_DATA, APP_WAITING_FOR_ACK);
    }

    xEventGroupSetBits(CAN_SEND_DATA, APP_CAN_SEND_DATA_BIT);
}

bool app_send_data_is_waiting_for_ack() {
    EventBits_t bits = xEventGroupGetBits(CAN_SEND_DATA);
    return bits & APP_WAITING_FOR_ACK;
}

void app_send_data_set_waiting_for_ack() {
    EventBits_t bits = xEventGroupGetBits(CAN_SEND_DATA);

    if ((bits & APP_CAN_SEND_DATA_BIT) != 0) {
        xEventGroupClearBits(CAN_SEND_DATA, APP_CAN_SEND_DATA_BIT);
    }

    xEventGroupSetBits(CAN_SEND_DATA, APP_WAITING_FOR_ACK);
}

void app_send_data_queue_task(void* pvParameters) {
    app_send_data_info_t data_to_send;

    while (true) {
        data_to_send = app_send_data_dequeue(portMAX_DELAY);

        xEventGroupWaitBits(
            CAN_SEND_DATA,
            APP_CAN_SEND_DATA_BIT, 
            pdTRUE, 
            pdFALSE, 
            portMAX_DELAY
        );

        if (!app_send_data_is_queue_empty()) {
            data_to_send = app_send_data_dequeue((TickType_t) 0); 
        }

        if (!app_send_data_is_waiting_for_ack()) {
            app_send_data_set_waiting_for_ack();
            app_encode_and_send_data(data_to_send);            
        }
    }
}

void app_create_send_data_task() {
    app_create_send_data_queue();

    xTaskCreatePinnedToCore(
        app_send_data_queue_task,
        "Send data task", 
        8192,
        NULL,
        4,
        NULL, 
        CONFIG_APP_QUEUE_TASK_CORE
    );
}

bool app_send_data_is_queue_empty() {
    return uxQueueMessagesWaiting(DATA_TO_SEND_QUEUE) == 0;
}

void app_send_data_enqueue(command_vehicle_type_t vehicle_type, float dir_y, float dir_x) {
    app_send_data_info_t data_to_push = {
        .vehicle_type = vehicle_type,
        .dir_x = dir_x,
        .dir_y = dir_y,
        .mac = (ROBOT_CONTROLLER_MAC)
    };

    if (xQueueOverwrite(DATA_TO_SEND_QUEUE, &data_to_push) != pdPASS) {
        ESP_LOGW(TAG, "Producer: Could not overwrite queue");
    }
}

app_send_data_info_t app_send_data_dequeue(TickType_t wait_ticks) {
    app_send_data_info_t received_data;

    if (xQueueReceive(DATA_TO_SEND_QUEUE, &received_data, wait_ticks) == pdTRUE) {
        ESP_LOGI(TAG, "Consumer: Dequeued data successfully.");
        return received_data;
    } 

    ESP_LOGW(TAG, "Consumer: Queue is empty. Returning default struct.");
    return (app_send_data_info_t){0};
}
