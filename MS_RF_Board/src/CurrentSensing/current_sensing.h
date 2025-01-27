/**
 * @file current_sensing.h
 * @brief Header file for Current Sensing Software Component (SWC) for MS_RF_Board.
 *
 * This header provides the declarations for monitoring current from two sensors,
 * SENSOR_1 and SENSOR_2, and interfaces to retrieve raw and debounced measurements.
 * It also defines a cyclic process to ensure regular updates of current readings.
 *
 * @date 2025-01-26
 */

#ifndef CURRENT_SENSING_H
#define CURRENT_SENSING_H

/**
 * @brief Current sensor enumeration.
 */
typedef enum {
    SENSOR_1,  // GPIO32
    SENSOR_2   // GPIO35
} current_sensor_t;

/**
 * @brief Initialize the current sensing module.
 *
 * Configures the ADC to read current measurements.
 */
void current_sensing_init(void);

/**
 * @brief Get the debounced current measurement from a specific sensor.
 *
 * Averages the last 5 current measurements to provide a stable reading.
 *
 * @param sensor The current sensor to retrieve debounced data from (SENSOR_1 or SENSOR_2).
 * @return The debounced load current in amperes.
 */
float current_sensing_get_debounced(current_sensor_t sensor);

/**
 * @brief Cyclically process current updates for all sensors.
 *
 * This function is called periodically to ensure the buffers for all sensors are updated.
 */
void current_sensing_process(void);

/**
 * @brief Set the app-defined current limit.
 *
 * @param limit The current limit in amperes.
 */
void current_sensing_set_limit(float limit);

/**
 * @brief Get the app-defined current limit.
 *
 * @return The app-defined current limit in amperes.
 */
float current_sensing_get_limit(void);

/**
 * @brief Deinitialize the current sensing module.
 *
 * Cleans up resources used by the current sensing module.
 */
void current_sensing_deinit(void);

#endif // CURRENT_SENSING_H
