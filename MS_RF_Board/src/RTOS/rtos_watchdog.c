/**
 * @file rtos_watchdog.c
 * @brief Watchdog Software Component (SWC) for MS_RF_Board.
 *
 * This module initializes and manages the watchdog functionality for the system,
 * ensuring tasks are running as expected and the system remains responsive.
 *
 * @date 2025-01-26
 */

#include "rtos_watchdog.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Initialize the Task Watchdog Timer (TWT).
 *
 * Configures the watchdog timer for periodic monitoring of tasks.
 *
 * @param timeout_s Watchdog timeout in seconds before triggering a reset.
 */
void rtos_watchdog_init(int timeout_s) {
    esp_task_wdt_config_t wdt_config = {
        .timeout_ms = timeout_s * 1000, // Convert seconds to milliseconds
        .trigger_panic = true,         // Enable panic handler on timeout
        .idle_core_mask = (1 << 0) | (1 << 1) // Monitor tasks on both cores
    };

    // Initialize the Task Watchdog Timer with the specified configuration
    esp_err_t err = esp_task_wdt_init(&wdt_config);
    if (err != ESP_OK) {
        printf("Watchdog initialization failed: %d\n", err);
    }

    // Add the current task to the watchdog monitor
    esp_task_wdt_add(NULL);
}

/**
 * @brief Feed the Task Watchdog Timer (TWT).
 *
 * Resets the watchdog timer to prevent system reset.
 */
void rtos_watchdog_feed() {
    esp_task_wdt_reset();
}

/**
 * @brief Add a task to the watchdog timer.
 *
 * Monitors an additional task by adding it to the Task Watchdog Timer.
 *
 * @param task_handle Handle of the task to be monitored.
 */
void rtos_watchdog_add_task(TaskHandle_t task_handle) {
    esp_err_t err = esp_task_wdt_add(task_handle);
    if (err != ESP_OK) {
        printf("Failed to add task to watchdog: %d\n", err);
    }
}

/**
 * @brief Remove a task from the watchdog timer.
 *
 * Stops monitoring a task by removing it from the Task Watchdog Timer.
 *
 * @param task_handle Handle of the task to be removed.
 */
void rtos_watchdog_remove_task(TaskHandle_t task_handle) {
    esp_err_t err = esp_task_wdt_delete(task_handle);
    if (err != ESP_OK) {
        printf("Failed to remove task from watchdog: %d\n", err);
    }
}
