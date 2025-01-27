/**
 * @file rtos.h
 * @brief Header file for RTOS Software Component (SWC) for MS_RF_Board.
 *
 * This header provides declarations for managing FreeRTOS tasks, including task creation,
 * task deletion, task delays, and logging task statistics.
 *
 * @date 2025-01-26
 */

#ifndef RTOS_H
#define RTOS_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdbool.h>
#include <stdint.h>

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
                      void *parameters, UBaseType_t priority, TaskHandle_t *handle, int core_id);

/**
 * @brief Delete a FreeRTOS task.
 *
 * Wrapper function to delete a task and log the operation.
 *
 * @param handle Handle of the task to delete. Use NULL to delete the calling task.
 */
void rtos_delete_task(TaskHandle_t handle);

/**
 * @brief Delay a task for a specified number of milliseconds.
 *
 * Wrapper for FreeRTOS delay with logging.
 *
 * @param milliseconds Delay time in milliseconds.
 */
void rtos_delay_ms(uint32_t milliseconds);

/**
 * @brief Log current FreeRTOS task statistics.
 *
 * Logs information about all currently running tasks.
 */
void rtos_log_task_stats(void);

#endif // RTOS_H
