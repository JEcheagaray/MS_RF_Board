/**
 * @file rtos_watchdog.h
 * @brief Header file for Watchdog Software Component (SWC) for MS_RF_Board.
 *
 * This header provides the declarations for watchdog functionality,
 * ensuring tasks are running as expected and the system remains responsive.
 *
 * @date 2025-01-26
 */

#ifndef RTOS_WATCHDOG_H
#define RTOS_WATCHDOG_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Initialize the Task Watchdog Timer (TWT).
 *
 * Configures the watchdog timer for periodic monitoring of tasks.
 *
 * @param timeout_s Watchdog timeout in seconds before triggering a reset.
 */
void rtos_watchdog_init(int timeout_s);

/**
 * @brief Feed the Task Watchdog Timer (TWT).
 *
 * Resets the watchdog timer to prevent system reset.
 */
void rtos_watchdog_feed(void);

/**
 * @brief Add a task to the watchdog timer.
 *
 * Monitors an additional task by adding it to the Task Watchdog Timer.
 *
 * @param task_handle Handle of the task to be monitored.
 */
void rtos_watchdog_add_task(TaskHandle_t task_handle);

/**
 * @brief Remove a task from the watchdog timer.
 *
 * Stops monitoring a task by removing it from the Task Watchdog Timer.
 *
 * @param task_handle Handle of the task to be removed.
 */
void rtos_watchdog_remove_task(TaskHandle_t task_handle);

/**
 * @brief Deinitialize the RTOS Watchdog.
 *
 * Stops the Task Watchdog Timer (TWDT) and removes tasks from its monitoring list.
 */
void rtos_watchdog_deinit(void);


#endif // RTOS_WATCHDOG_H
