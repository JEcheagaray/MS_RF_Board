/**
 * @file bluetooth.h
 * @brief Header file for Bluetooth Software Component (SWC) for MS_RF_Board.
 *
 * This header provides the declarations for the Bluetooth Low Energy (BLE)
 * communication functionalities of the MS_RF_Board.
 *
 * @date 2025-01-26
 */

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

/**
 * @brief Initialize the Bluetooth module.
 *
 * Configures and starts the BLE stack for communication.
 */
void bluetooth_init(void);

/**
 * @brief Run Bluetooth communication tasks.
 *
 * Handles periodic Bluetooth operations and message processing.
 */
void bluetooth_run(void);

/**
 * @brief Deinitialize the Bluetooth module.
 *
 * Stops and releases all Bluetooth resources.
 */
void bluetooth_deinit(void);

#endif // BLUETOOTH_H
