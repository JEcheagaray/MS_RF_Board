/**
 * @file diagnostics.h
 * @brief Header file for Diagnostics Software Component (SWC) for MS_RF_Board.
 *
 * This header provides the declarations for diagnostics and logging functionalities
 * to monitor the system's health, performance, and errors.
 *
 * @date 2025-01-26
 */

#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <stddef.h> // For size_t

/**
 * @brief Initialize the diagnostics module.
 *
 * Configures logging settings and prepares diagnostics for runtime monitoring.
 */
void diagnostics_init(void);

/**
 * @brief Run diagnostics tasks.
 *
 * Monitors the system's state, logs important runtime information, and detects
 * potential issues (e.g., task overflows, resource limits).
 */
void diagnostics_run(void);

/**
 * @brief Deinitialize the diagnostics module.
 *
 * Cleans up diagnostics resources and stops monitoring.
 */
void diagnostics_deinit(void);

#endif // DIAGNOSTICS_H
