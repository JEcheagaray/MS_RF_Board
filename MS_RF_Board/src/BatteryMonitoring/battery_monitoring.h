/**
 * @file battery_monitoring.h
 * @brief Header file for Battery Monitoring Software Component (SWC) for MS_RF_Board.
 *
 * This header provides declarations for monitoring the battery voltage and calculating
 * the state of charge (SOC) for the MS_RF_Board using three 18650 batteries in series.
 *
 * @date 2025-01-26
 */

#ifndef BATTERY_MONITORING_H
#define BATTERY_MONITORING_H

/**
 * @brief Initialize the battery monitoring module.
 *
 * Configures the ADC to read the battery voltage.
 */
void battery_monitoring_init(void);

/**
 * @brief Run the battery monitoring task.
 *
 * Reads the battery voltage, calculates the SOC, and logs the battery status.
 */
void battery_monitoring_run(void);

/**
 * @brief Deinitialize the battery monitoring module.
 *
 * Cleans up resources used by the battery monitoring module.
 */
void battery_monitoring_deinit(void);

#endif // BATTERY_MONITORING_H
