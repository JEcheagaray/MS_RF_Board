/**
 * @file bluetooth.c
 * @brief Bluetooth Classic implementation for MS_RF_Board.
 *
 * This module sets up Bluetooth Classic using the SPP profile for serial communication.
 * The module supports basic initialization, data handling, and deinitialization.
 *
 * @date 2025-01-26
 */

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_bt_gap.h"
#include "esp_spp_api.h"
#include "esp_log.h"

#define TAG "BluetoothSPP"

static const char *device_name = "MS_RF_Board";

/**
 * @brief Event handler for SPP events.
 *
 * Handles various SPP events such as initialization, data reception, and data transmission.
 *
 * @param event SPP event type.
 * @param param Event parameters.
 */
static void spp_event_handler(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    switch (event) {
        case ESP_SPP_INIT_EVT:
            ESP_LOGI(TAG, "ESP SPP initialized.");
            esp_bt_gap_set_device_name(device_name);  // Use updated API
            esp_spp_start_srv(ESP_SPP_SEC_AUTHENTICATE, ESP_SPP_ROLE_SLAVE, 0, "SPP_SERVER");
            break;

        case ESP_SPP_START_EVT:
            ESP_LOGI(TAG, "SPP server started.");
            break;

        case ESP_SPP_DATA_IND_EVT:
            ESP_LOGI(TAG, "Data received: %.*s", param->data_ind.len, (char *)param->data_ind.data);
            // Handle received data
            break;

        case ESP_SPP_WRITE_EVT:
            ESP_LOGI(TAG, "Data sent successfully.");
            break;

        default:
            ESP_LOGI(TAG, "Unhandled SPP event: %d", event);
            break;
    }
}

/**
 * @brief Initialize Bluetooth Classic with SPP.
 *
 * Configures and enables the Bluetooth Classic stack using the SPP profile.
 */
void bluetooth_init() {
    esp_err_t ret;

    ESP_LOGI(TAG, "Initializing Bluetooth...");

    // Release BLE memory (if enabled)
    ret = esp_bt_controller_mem_release(ESP_BT_MODE_BLE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to release BLE memory: %s", esp_err_to_name(ret));
    }

    // Initialize Bluetooth controller
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize BT controller: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable BT controller: %s", esp_err_to_name(ret));
        return;
    }

    // Initialize Bluedroid stack
    ret = esp_bluedroid_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize Bluedroid stack: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable Bluedroid stack: %s", esp_err_to_name(ret));
        return;
    }

    // Register SPP callback
    ret = esp_spp_register_callback(spp_event_handler);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register SPP callback: %s", esp_err_to_name(ret));
        return;
    }

    // Initialize SPP with configuration
    esp_spp_cfg_t spp_config = {
        .mode = ESP_SPP_MODE_CB,          // Callback mode
        .enable_l2cap_ertm = false,      // Disable L2CAP enhanced retransmission mode
        .tx_buffer_size = 0              // Default buffer size
    };
    ret = esp_spp_enhanced_init(&spp_config);  // Use the updated API
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPP: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "Bluetooth initialized successfully.");
}

/**
 * @brief Deinitialize Bluetooth Classic.
 *
 * Cleans up and releases resources used by the Bluetooth Classic stack.
 */
void bluetooth_deinit() {
    ESP_LOGI(TAG, "Deinitializing Bluetooth...");
    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    ESP_LOGI(TAG, "Bluetooth deinitialized.");
}
