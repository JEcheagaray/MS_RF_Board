/**
 * @file rtos.c
 * @brief RTOS Software Component (SWC) for MS_RF_Board.
 *
 * This module provides utilities for managing FreeRTOS tasks, including task creation,
 * task monitoring, and scheduling periodic tasks.
 *
 * @date 2025-01-26
 */

#include "rtos.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define TAG "RTOS"

/**
 * @brief Create a FreeRTOS task.
 *
 * Wrapper function to create a task with error handling and logging.
 *
 * @param task_function The function to execute as a task.
 * @param task_name Name of the task for debugging purposes.
 * @param stack_depth Stack depth for the task.
 * @param parameters Parameters to pass to the task function.
 * @param priority Task priority.
 * @param handle Pointer to store the created task's handle.
 * @param core_id Core to pin the task to (-1 for no pinning).
 * @return true if the task was created successfully, false otherwise.
 */
bool rtos_create_task(TaskFunction_t task_function, const char *task_name, uint32_t stack_depth,
                      void *parameters, UBaseType_t priority, TaskHandle_t *handle, int core_id) {
    BaseType_t result = xTaskCreatePinnedToCore(task_function, task_name, stack_depth,
                                                parameters, priority, handle, core_id);
    if (result == pdPASS) {
        ESP_LOGI(TAG, "Task '%s' created successfully.", task_name);
        return true;
    } else {
        ESP_LOGE(TAG, "Failed to create task '%s'.", task_name);
        return false;
    }
}

/**
 * @brief Delete a FreeRTOS task.
 *
 * Wrapper function to delete a task and log the operation.
 *
 * @param handle Handle of the task to delete. Use NULL to delete the calling task.
 */
void rtos_delete_task(TaskHandle_t handle) {
    vTaskDelete(handle);
    ESP_LOGI(TAG, "Task deleted successfully.");
}

/**
 * @brief Delay a task for a specified number of milliseconds.
 *
 * Wrapper for FreeRTOS delay with logging.
 *
 * @param milliseconds Delay time in milliseconds.
 */
void rtos_delay_ms(uint32_t milliseconds) {
    ESP_LOGI(TAG, "Delaying task for %lu ms.", (unsigned long)milliseconds);
    vTaskDelay(pdMS_TO_TICKS(milliseconds));
}


/**
 * @brief Log current FreeRTOS task statistics.
 *
 * Logs information about all currently running tasks.
 */
void rtos_log_task_stats() {
    char *buffer = pvPortMalloc(1024);
    if (buffer) {
        vTaskList(buffer);
        ESP_LOGI(TAG, "Task statistics:\n%s", buffer);
        vPortFree(buffer);
    } else {
        ESP_LOGE(TAG, "Failed to allocate memory for task statistics.");
    }
}
