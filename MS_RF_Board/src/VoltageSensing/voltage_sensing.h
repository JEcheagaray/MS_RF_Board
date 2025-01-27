/**
 * @file voltage_sensing.h
 * @brief Header file for Voltage Sensing Software Component (SWC) for MS_RF_Board.
 *
 * This header provides declarations for monitoring voltage using the VS_VOUT_1 sensor (GPIO33).
 * The module supports raw and debounced measurements.
 *
 * Note: The sensor pin VS_VOUT_2 (GPIO25) was not used because it is connected to ADC2,
 * which may cause conflicts when Wi-Fi is active. Only VS_VOUT_1 (GPIO33) is used
 * for consistent and reliable voltage sensing.
 *
 * @date 2025-01-26
 */

#ifndef VOLTAGE_SENSING_H
#define VOLTAGE_SENSING_H

#include <stdint.h>

/**
 * @brief Initialize the voltage sensing module.
 *
 * Configures the ADC to read voltage measurements and sets up calibration.
 */
void voltage_sensing_init(void);

/**
 * @brief Get the debounced voltage measurement.
 *
 * Averages the last 5 voltage measurements to provide a stable reading.
 *
 * @return The debounced load voltage in volts.
 */
float voltage_sensing_get_debounced(void);

/**
 * @brief Update the debounced voltage buffer.
 *
 * Reads the raw voltage and updates the buffer for debouncing.
 */
void voltage_sensing_update(void);

/**
 * @brief Run the voltage sensing task.
 *
 * Updates the debounced buffer and logs the voltage.
 */
void voltage_sensing_run(void);

/**
 * @brief Deinitialize the voltage sensing module.
 *
 * Cleans up resources used by the voltage sensing module.
 */
void voltage_sensing_deinit(void);

#endif // VOLTAGE_SENSING_H
