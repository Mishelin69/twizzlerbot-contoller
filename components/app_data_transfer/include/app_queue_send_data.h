#pragma once

#include <freertos/idf_additions.h>

#include <enums/command_vehicle_type_enum.h>
#include <models/app_send_data_model.h>

#define APP_SEND_DATA_TASK_FREQUENCY (100)
#define CONFIG_APP_QUEUE_TASK_CORE 0

#define APP_CAN_SEND_DATA_BIT BIT0
#define APP_WAITING_FOR_ACK BIT1

void app_create_send_data_queue();

void app_send_data_queue_task(void* pvParameters);

void app_create_send_data_task();

void app_send_data_enqueue(command_vehicle_type_t vehicle_type, float dir_y, float dir_x);

bool app_send_data_is_queue_empty();

app_send_data_info_t app_send_data_dequeue(TickType_t wait_ticks);

bool app_send_data_create_event_group();

bool app_send_data_is_waiting_for_ack();

void app_send_data_send_ack_signal();

void app_send_data_set_waiting_for_ack();
