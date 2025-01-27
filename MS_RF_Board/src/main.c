/**
 * @file main.c
 * @brief Main application entry point for MS_RF_Board.
 *
 * This file initializes the system, creates FreeRTOS tasks, and assigns them to cores.
 * Tasks are grouped by periodicity and handle specific functionalities.
 *
 * @author Jorge
 * @date 2025-01-26
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_watchdog.h" // Include Watchdog header

// Include headers for all SWCs
#include "bluetooth.h"
#include "diagnostics.h"
#include "command_parser.h"
#include "gate_driver_control.h"
#include "voltage_sensing.h"
#include "current_sensing.h"
#include "battery_monitoring.h"
#include "error_handling.h"
#include "nvm.h"
#include "rtos.h"
#include "system_config.h"

// Task handles
TaskHandle_t task_100ms_core_0_handle;
TaskHandle_t task_10000ms_core_0_handle;
TaskHandle_t task_1ms_core_1_handle;
TaskHandle_t task_10ms_core_1_handle;
TaskHandle_t task_1000ms_core_1_handle;

/**
 * @brief Task for handling Bluetooth communication (Core 0, 100 ms).
 * 
 * Initializes Bluetooth functionality and manages periodic communication with the mobile app.
 *
 * @param params Task parameters (unused).
 */
void task_100ms_core_0(void *params) {
    bluetooth_init();
    while (1) {
        bluetooth_run();  ///< Run Bluetooth communication.
        rtos_watchdog_feed(); // Feed the watchdog
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**
 * @brief Task for handling diagnostics and logging (Core 0, 10,000 ms).
 * 
 * Manages system diagnostics, error logging, and status reporting.
 *
 * @param params Task parameters (unused).
 */
void task_10000ms_core_0(void *params) {
    diagnostics_init();
    while (1) {
        diagnostics_run();  ///< Run diagnostics and logging.
        rtos_watchdog_feed(); // Feed the watchdog
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

/**
 * @brief Task for controlling the gate driver (Core 1, 1 ms).
 * 
 * Generates gate signals for the MOSFET to control output frequency and voltage.
 *
 * @param params Task parameters (unused).
 */
void task_1ms_core_1(void *params) {
    gate_driver_control_init();
    while (1) {
        gate_driver_control_run();  ///< Run gate driver control.
        rtos_watchdog_feed(); // Feed the watchdog
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

/**
 * @brief Task for voltage and current sensing (Core 1, 10 ms).
 * 
 * Monitors and regulates output voltage and current to ensure safe operation.
 *
 * @param params Task parameters (unused).
 */
void task_10ms_core_1(void *params) {
    voltage_sensing_init();
    current_sensing_init();
    while (1) {
        voltage_sensing_run();  ///< Run voltage sensing and regulation.
        current_sensing_run();  ///< Run current sensing and safety monitoring.
        rtos_watchdog_feed(); // Feed the watchdog
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/**
 * @brief Task for monitoring battery state of charge (Core 1, 1,000 ms).
 * 
 * Reads the battery voltage to determine the state of charge (SOC).
 *
 * @param params Task parameters (unused).
 */
void task_1000ms_core_1(void *params) {
    battery_monitoring_init();
    while (1) {
        battery_monitoring_run();  ///< Run battery monitoring.
        rtos_watchdog_feed(); // Feed the watchdog
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief Main application entry point.
 * 
 * Initializes the system, sets up non-volatile memory, and creates FreeRTOS tasks
 * for both cores.
 */
void app_main() {
    // Initialize system configuration
    system_config_init();

    // Initialize non-volatile memory
    nvm_init();

    // Initialize the watchdog with a 5-second timeout
    rtos_watchdog_init(5);

    // Create tasks for Core 0
    xTaskCreatePinnedToCore(task_100ms_core_0, "Task_100ms_Core_0", 2048, NULL, 1, &task_100ms_core_0_handle, 0);
    xTaskCreatePinnedToCore(task_10000ms_core_0, "Task_10000ms_Core_0", 2048, NULL, 1, &task_10000ms_core_0_handle, 0);

    // Create tasks for Core 1
    xTaskCreatePinnedToCore(task_1ms_core_1, "Task_1ms_Core_1", 2048, NULL, 2, &task_1ms_core_1_handle, 1);
    xTaskCreatePinnedToCore(task_10ms_core_1, "Task_10ms_Core_1", 2048, NULL, 1, &task_10ms_core_1_handle, 1);
    xTaskCreatePinnedToCore(task_1000ms_core_1, "Task_1000ms_Core_1", 2048, NULL, 1, &task_1000ms_core_1_handle, 1);

    printf("System initialized. Tasks running.\n");
}
