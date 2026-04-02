#include "app_nvs.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include <string.h>

#define TAG "app_nvs"

app_nvs_return_t app_nvs_init() {
    ESP_LOGI(TAG, "initializing flash");
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        return APP_NVS_OPERATION_FAILED;
    }

    return APP_NVS_SUCCESS;
}

app_nvs_return_t app_nvs_handle_non_okay_return(esp_err_t error) {
    switch (error) {
        case ESP_ERR_INVALID_ARG:
            return APP_NVS_INVALID_INPUT;
        case ESP_ERR_NVS_NOT_FOUND:
        case ESP_ERR_NVS_INVALID_NAME:
            return APP_NVS_NOT_FOUND;
        case ESP_ERR_NOT_ALLOWED:
            return APP_NVS_INVALID_MODE;
        default:
            return APP_NVS_OPERATION_FAILED;
    }
}

app_nvs_return_t app_nvs_entry_exists(const char* namespace, const char* entry_name) {
    nvs_handle_t handle;
    app_nvs_return_t status = app_nvs_entry_exists_with_handle(namespace, entry_name, &handle);

    if (status != APP_NVS_SUCCESS) {
        nvs_close(handle);
        return status;
    }

    nvs_close(handle);
    return status;
}

app_nvs_return_t app_nvs_entry_exists_with_handle(const char* namespace, const char* entry_name, nvs_handle_t* handle) {
    if (!entry_name) {
        return APP_NVS_INVALID_INPUT;
    }

    esp_err_t status = nvs_open(namespace, NVS_READONLY, handle);

    if (status != ESP_OK) {
        ESP_LOGE(TAG, "Couldn't open namespace %s", namespace);
        return app_nvs_handle_non_okay_return(status);
    }

    nvs_type_t type;
    status = nvs_find_key(*handle, entry_name, &type);

    if (status != ESP_OK) {
        ESP_LOGE(TAG, "Key %s doesnt exist", entry_name);
        return app_nvs_handle_non_okay_return(status);
    }

    return APP_NVS_SUCCESS;
}

app_nvs_return_t app_nvs_is_read_valid(char* buffer, size_t buffer_size, size_t required_size) {
    if (buffer_size < required_size) {
        return APP_NVS_INVALID_INPUT;
    }

    return APP_NVS_SUCCESS;
}

app_nvs_return_t app_nvs_read_str(const char* namespace, const char* entry_name, char* buffer, size_t buffer_size) {
    nvs_handle_t handle;
    app_nvs_return_t status = app_nvs_entry_exists_with_handle(namespace, entry_name, &handle);

    if (status != APP_NVS_SUCCESS) {
        nvs_close(handle);
        return status; 
    }

    size_t required_size = 0;
    esp_err_t error = nvs_get_str(handle, entry_name, NULL, &required_size);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Error while getting string length for key %s", entry_name);
        nvs_close(handle);
        return app_nvs_handle_non_okay_return(error);
    }

    if (app_nvs_is_read_valid(buffer, buffer_size, required_size) != APP_NVS_SUCCESS) {
        ESP_LOGE(TAG, "Error, invalid buffer size vs required_size!", entry_name);
        nvs_close(handle);
        return APP_NVS_OPERATION_FAILED;
    }

    error = nvs_get_str(handle, entry_name, buffer, &required_size);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Error while reading from key: %s", entry_name);
        nvs_close(handle);
        return app_nvs_handle_non_okay_return(error);
    }

    ESP_LOGI(TAG, "Succesfully read key: %s with value: %s", entry_name, buffer);

    nvs_close(handle);
    return APP_NVS_SUCCESS;
}

app_nvs_return_t app_nvs_write_str(const char* namespace, const char* entry_name, char* buffer) {
    nvs_handle_t handle;
    esp_err_t error = nvs_open(namespace, NVS_READWRITE, &handle);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Couldn't open entry name for write: %s", entry_name);
        return app_nvs_handle_non_okay_return(error);
    }

    error = nvs_set_str(handle, entry_name, buffer);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Error while trying to write entry name: %s with data:\n%s", entry_name, buffer);
        nvs_close(handle);
        return app_nvs_handle_non_okay_return(error);
    }

    ESP_LOGI(TAG, "Succesfully written key: %s with value: %s", entry_name, buffer);
    error = nvs_commit(handle);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Failed to commit NVS changes");
    }

    nvs_close(handle);
    return APP_NVS_SUCCESS;
}

app_nvs_return_t app_nvs_read_blob(const char* namespace, const char* entry_name, void* buffer, size_t buffer_size) {
    nvs_handle_t handle;
    app_nvs_return_t status = app_nvs_entry_exists_with_handle(namespace, entry_name, &handle);

    if (status != APP_NVS_SUCCESS) {
        nvs_close(handle);
        return status; 
    }

    size_t required_size = 0;
    esp_err_t error = nvs_get_blob(handle, entry_name, NULL, &required_size);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Error while getting blob length for key %s", entry_name);
        nvs_close(handle);
        return app_nvs_handle_non_okay_return(error);
    }

    if (app_nvs_is_read_valid(buffer, buffer_size, required_size) != APP_NVS_SUCCESS) {
        ESP_LOGE(TAG, "Error, invalid buffer size vs required_size!", entry_name);
        nvs_close(handle);
        return APP_NVS_OPERATION_FAILED;
    }

    error = nvs_get_blob(handle, entry_name, buffer, &required_size);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Error while reading from key: %s", entry_name);
        nvs_close(handle);
        return app_nvs_handle_non_okay_return(error);
    }

    ESP_LOGI(TAG, "Succesfully read key: %s", entry_name);

    nvs_close(handle);
    return APP_NVS_SUCCESS;

}

app_nvs_return_t app_nvs_write_blob(const char* namespace, const char* entry_name, void* buffer, size_t buffer_size) {
    nvs_handle_t handle;
    esp_err_t error = nvs_open(namespace, NVS_READWRITE, &handle);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Couldn't open entry name for write: %s", entry_name);
        return app_nvs_handle_non_okay_return(error);
    }

    error = nvs_set_blob(handle, entry_name, buffer, buffer_size);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Error while trying to write entry name: %s", entry_name);
        nvs_close(handle);
        return app_nvs_handle_non_okay_return(error);
    }

    ESP_LOGI(TAG, "Succesfully written key: %s", entry_name);
    error = nvs_commit(handle);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Failed to commit NVS changes");
    }

    nvs_close(handle);

    return APP_NVS_SUCCESS;
}

app_nvs_return_t app_nvs_get_blob_size(const char* namespace, const char* entry_name, size_t* buffer_size) {
    nvs_handle_t handle;
    app_nvs_return_t status = app_nvs_entry_exists_with_handle(namespace, entry_name, &handle);

    if (status != APP_NVS_SUCCESS) {
        nvs_close(handle);
        return status; 
    }

    esp_err_t error = nvs_get_blob(handle, entry_name, NULL, buffer_size);

    if (error != ESP_OK) {
        ESP_LOGE(TAG, "Error while getting blob length for key %s", entry_name);
        nvs_close(handle);
        return app_nvs_handle_non_okay_return(error);
    }

    return APP_NVS_SUCCESS;
}
