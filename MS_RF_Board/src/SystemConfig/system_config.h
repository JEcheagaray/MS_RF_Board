/**
 * @file system_config.h
 * @brief Header file for System Configuration Software Component (SWC) for MS_RF_Board.
 *
 * This header provides declarations for system initialization and deinitialization,
 * ensuring all components are properly configured and cleaned up.
 *
 * @date 2025-01-26
 */

#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

/**
 * @brief Initialize the system configuration.
 *
 * This function initializes all system components and ensures they are ready for operation.
 */
void system_config_init(void);

/**
 * @brief Deinitialize the system configuration.
 *
 * This function deinitializes all system components and cleans up resources.
 */
void system_config_deinit(void);

#endif // SYSTEM_CONFIG_H