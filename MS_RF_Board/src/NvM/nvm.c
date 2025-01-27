/**
 * @file nvm.c
 * @brief Non-Volatile Memory (NVM) Software Component (SWC) for MS_RF_Board.
 *
 * This module provides functionality to store and retrieve persistent data
 * such as user configurations, frequency programs, and settings.
 *
 * @date 2025-01-26
 */

#include "nvm.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

#define TAG "NVM"
#define NVM_NAMESPACE "storage"

/**
 * @brief Initialize the NVM module.
 *
 * Prepares the non-volatile storage for reading and writing data.
 */
void nvm_init() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "Erasing and reinitializing NVS due to errors...");
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize NVS: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "NVM initialized successfully.");
    }
}

/**
 * @brief Save a key-value pair to NVM.
 *
 * @param key The key under which the value is stored.
 * @param value The string value to store.
 * @return true if the operation is successful, false otherwise.
 */
bool nvm_save(const char *key, const char *value) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVM_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(err));
        return false;
    }

    err = nvs_set_str(handle, key, value);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
        nvs_close(handle);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "Key '%s' saved successfully.", key);
            return true;
        }
    }

    ESP_LOGE(TAG, "Failed to save key '%s': %s", key, esp_err_to_name(err));
    nvs_close(handle);
    return false;
}

/**
 * @brief Load a value from NVM.
 *
 * @param key The key associated with the value.
 * @param value Buffer to store the retrieved value.
 * @param length Length of the buffer.
 * @return true if the operation is successful, false otherwise.
 */
bool nvm_load(const char *key, char *value, size_t length) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVM_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(err));
        return false;
    }

    err = nvs_get_str(handle, key, value, &length);
    nvs_close(handle);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Key '%s' loaded successfully.", key);
        return true;
    } else {
        ESP_LOGE(TAG, "Failed to load key '%s': %s", key, esp_err_to_name(err));
        return false;
    }
}

/**
 * @brief Deinitialize the NVM module.
 *
 * Cleans up resources used by the NVM module.
 */
void nvm_deinit() {
    ESP_LOGI(TAG, "Deinitializing NVM module...");
    nvs_flash_deinit();
    ESP_LOGI(TAG, "NVM module deinitialized.");
}
