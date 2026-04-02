#pragma once

#include "enums/app_nvs_return_status_enum.h"
#include "esp_err.h"
#include "nvs.h"

app_nvs_return_t app_nvs_init();

app_nvs_return_t app_nvs_handle_non_okay_return(esp_err_t error);

app_nvs_return_t app_nvs_entry_exists(const char* namespace, const char* entry_name);

app_nvs_return_t app_nvs_entry_exists_with_handle(const char* namespace, const char* entry_name, nvs_handle_t* handle);

app_nvs_return_t app_nvs_is_read_valid(char* buffer, size_t buffer_size, size_t required_size);

app_nvs_return_t app_nvs_read_str(const char* namespace, const char* entry_name, char* buffer, size_t buffer_size);

app_nvs_return_t app_nvs_write_str(const char* namespace, const char* entry_name, char* buffer);

app_nvs_return_t app_nvs_read_blob(const char* namespace, const char* entry_name, void* buffer, size_t buffer_size);

app_nvs_return_t app_nvs_write_blob(const char* namespace, const char* entry_name, void* buffer, size_t buffer_size);

app_nvs_return_t app_nvs_get_blob_size(const char* namespace, const char* entry_name, size_t* buffer_size);
