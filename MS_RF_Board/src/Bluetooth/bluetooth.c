/**
 * @file bluetooth.c
 * @brief Bluetooth Software Component (SWC) for MS_RF_Board.
 *
 * This module manages Bluetooth Low Energy (BLE) communication
 * between the MS_RF_Board and the mobile application.
 *
 * @author Jorge
 * @date 2025-01-26
 */

#include "bluetooth.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatt_common_api.h"
#include "esp_gatts_api.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "Bluetooth"

/**
 * @brief Initialize the Bluetooth module.
 *
 * Configures and starts the BLE stack for communication.
 */
void bluetooth_init() {
    esp_err_t ret;

    ESP_LOGI(TAG, "Initializing Bluetooth...");

    // Initialize the Bluetooth controller
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluetooth controller initialization failed: %s", esp_err_to_name(ret));
        return;
    }

    // Enable the Bluetooth controller
    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluetooth controller enable failed: %s", esp_err_to_name(ret));
        return;
    }

    // Initialize the Bluedroid stack
    ret = esp_bluedroid_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluedroid stack initialization failed: %s", esp_err_to_name(ret));
        return;
    }

    // Enable the Bluedroid stack
    ret = esp_bluedroid_enable();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluedroid stack enable failed: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "Bluetooth initialized successfully.");
}

/**
 * @brief Run Bluetooth communication tasks.
 *
 * Handles periodic Bluetooth operations and message processing.
 */
void bluetooth_run() {
    // Placeholder for actual BLE communication logic
    ESP_LOGI(TAG, "Bluetooth task running.");
}

/**
 * @brief Deinitialize the Bluetooth module.
 *
 * Stops and releases all Bluetooth resources.
 */
void bluetooth_deinit() {
    esp_err_t ret;

    ESP_LOGI(TAG, "Deinitializing Bluetooth...");

    ret = esp_bluedroid_disable();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluedroid stack disable failed: %s", esp_err_to_name(ret));
    }

    ret = esp_bluedroid_deinit();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluedroid stack deinitialization failed: %s", esp_err_to_name(ret));
    }

    ret = esp_bt_controller_disable();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluetooth controller disable failed: %s", esp_err_to_name(ret));
    }

    ret = esp_bt_controller_deinit();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluetooth controller deinitialization failed: %s", esp_err_to_name(ret));
    }

    ESP_LOGI(TAG, "Bluetooth deinitialized successfully.");
}
