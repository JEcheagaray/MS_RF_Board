/**
 * @file error_handling.c
 * @brief Error Handling Software Component (SWC) for MS_RF_Board.
 *
 * This module handles system errors, logging, and recovery mechanisms to ensure
 * stability and prevent system crashes.
 *
 * @date 2025-01-26
 */

#include "error_handling.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "ErrorHandling"

/**
 * @brief Initialize the error handling module.
 *
 * Sets up error logging and recovery mechanisms.
 */
void error_handling_init() {
    ESP_LOGI(TAG, "Initializing error handling module...");

    // Additional setup for error monitoring can be added here.

    ESP_LOGI(TAG, "Error handling module initialized successfully.");
}

/**
 * @brief Log an error message.
 *
 * Logs the provided error message with the ESP logging system.
 *
 * @param error_message The error message to log.
 */
void error_handling_log(const char *error_message) {
    if (error_message == NULL) {
        ESP_LOGE(TAG, "Attempted to log a null error message.");
        return;
    }

    ESP_LOGE(TAG, "Error: %s", error_message);
}

/**
 * @brief Perform recovery actions for critical errors.
 *
 * Attempts to recover the system or gracefully shut down in the event of a critical error.
 *
 * @param error_code The error code associated with the critical error.
 */
void error_handling_recover(int error_code) {
    ESP_LOGE(TAG, "Critical error encountered. Error code: %d", error_code);

    // Example recovery mechanism: restart the system
    if (error_code == ERROR_CODE_CRITICAL) {
        ESP_LOGE(TAG, "Restarting system due to critical error...");
        esp_restart();
    }

    // Additional recovery actions can be added here.
}

/**
 * @brief Deinitialize the error handling module.
 *
 * Cleans up resources used by the error handling module.
 */
void error_handling_deinit() {
    ESP_LOGI(TAG, "Deinitializing error handling module...");

    // Additional cleanup can be added here.

    ESP_LOGI(TAG, "Error handling module deinitialized.");
}
