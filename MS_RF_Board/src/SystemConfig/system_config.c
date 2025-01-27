/**
 * @file system_config.c
 * @brief System Configuration Software Component (SWC) for MS_RF_Board.
 *
 * This module handles the initialization and deinitialization of the entire system.
 * It sets up critical configurations and ensures all components are ready for operation.
 *
 * @date 2025-01-26
 */

#include "system_config.h"
#include "esp_log.h"
#include "bluetooth.h"
#include "diagnostics.h"
#include "command_parser.h"
//#include "gate_driver_control.h"
#include "voltage_sensing.h"
#include "current_sensing.h"
#include "battery_monitoring.h"
#include "error_handling.h"
#include "nvm.h"
#include "rtos_watchdog.h"

#define TAG "SystemConfig"

/**
 * @brief Initialize the system configuration.
 *
 * This function initializes all system components and ensures they are ready for operation.
 */
void system_config_init() {
    ESP_LOGI(TAG, "Initializing system configuration...");

    // Initialize Non-Volatile Memory
    nvm_init();

    // Initialize Error Handling
    error_handling_init();

    // Initialize Bluetooth
    bluetooth_init();

    // Initialize Diagnostics
    diagnostics_init();

    // Initialize Command Parser
    command_parser_init();

    // Initialize Gate Driver Control
    //gate_driver_control_init();

    // Initialize Voltage Sensing
    voltage_sensing_init();

    // Initialize Current Sensing
    current_sensing_init();

    // Initialize Battery Monitoring
    battery_monitoring_init();

    // Initialize RTOS Watchdog
    rtos_watchdog_init(5);  // Set a 5-second timeout for the watchdog

    ESP_LOGI(TAG, "System configuration initialized successfully.");
}

/**
 * @brief Deinitialize the system configuration.
 *
 * This function deinitializes all system components and cleans up resources.
 */
void system_config_deinit() {
    ESP_LOGI(TAG, "Deinitializing system configuration...");

    // Deinitialize all components
    rtos_watchdog_deinit();
    battery_monitoring_deinit();
    current_sensing_deinit();
    voltage_sensing_deinit();
    //gate_driver_control_deinit();
    command_parser_deinit();
    diagnostics_deinit();
    bluetooth_deinit();
    error_handling_deinit();
    nvm_deinit();

    ESP_LOGI(TAG, "System configuration deinitialized successfully.");
}
