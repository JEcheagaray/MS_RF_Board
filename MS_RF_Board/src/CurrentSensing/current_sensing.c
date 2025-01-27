/**
 * @file current_sensing.c
 * @brief Current Sensing Software Component (SWC) for MS_RF_Board.
 *
 * This module monitors the current passing through the load and provides an interface
 * for the gate driver module to retrieve raw and debounced current measurements.
 * The gate driver uses this information to decide when to disable or enable the gate.
 *
 * @date 2025-01-26
 */

#include "current_sensing.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define TAG "CurrentSensing"

// ADC Configuration
#define ADC_MAX_VALUE 4095
#define DEFAULT_VREF 1100  // Default reference voltage in mV
#define CURRENT_SENSE_RESISTOR 0.015  // Shunt resistor value in ohms (15 mΩ)
#define CURRENT_SENSE_GAIN 20.0       // Amplification gain of the current sensing circuit

// Safety Thresholds
#define CURRENT_LIMIT_SAFE 0.1  // 100 mA human safety limit
static float app_current_limit = CURRENT_LIMIT_SAFE;  // Default to safety limit
static float last_measurements[2][5] = {{0}, {0}};  // Buffers for debouncing (Sensor1 and Sensor2)
static int measurement_index[2] = {0, 0};

static esp_adc_cal_characteristics_t *adc_chars;  // ADC calibration characteristics

/**
 * @brief Initialize the current sensing module.
 *
 * Configures the ADC to read current measurements and sets up calibration.
 */
void current_sensing_init() {
    ESP_LOGI(TAG, "Initializing current sensing module...");

    adc1_config_width(ADC_WIDTH_BIT_12);  // 12-bit ADC resolution
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);  // Sensor 1: GPIO32
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);  // Sensor 2: GPIO35

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    ESP_LOGI(TAG, "Current sensing module initialized successfully.");
}

/**
 * @brief Read the raw load current from a specific sensor.
 *
 * Converts the ADC reading to the actual load current in amperes.
 *
 * @param sensor The current sensor to read from (SENSOR_1 or SENSOR_2).
 * @return The raw load current in amperes.
 */
static float read_raw_current(current_sensor_t sensor) {
    int adc_reading = 0;

    if (sensor == SENSOR_1) {
        adc_reading = adc1_get_raw(ADC1_CHANNEL_4);  // GPIO32
    } else if (sensor == SENSOR_2) {
        adc_reading = adc1_get_raw(ADC1_CHANNEL_7);  // GPIO35
    }

    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);  // Calibrated voltage in mV
    float current = ((float)voltage / 1000.0) / (CURRENT_SENSE_RESISTOR * CURRENT_SENSE_GAIN);  // Current in amperes
    return current;
}

/**
 * @brief Get the debounced current measurement from a specific sensor.
 *
 * Averages the last 5 current measurements to provide a stable reading.
 *
 * @param sensor The current sensor to retrieve debounced data from (SENSOR_1 or SENSOR_2).
 * @return The debounced load current in amperes.
 */
float current_sensing_get_debounced(current_sensor_t sensor) {
    float sum = 0;
    int index = (sensor == SENSOR_1) ? 0 : 1;

    for (int i = 0; i < 5; i++) {
        sum += last_measurements[index][i];
    }
    return sum / 5.0;
}

/**
 * @brief Update the debounced current buffer for a specific sensor.
 *
 * Reads the raw current and updates the buffer for debouncing.
 *
 * @param sensor The current sensor to update (SENSOR_1 or SENSOR_2).
 */
void current_sensing_update(current_sensor_t sensor) {
    float current = read_raw_current(sensor);
    int index = (sensor == SENSOR_1) ? 0 : 1;

    last_measurements[index][measurement_index[index]] = current;
    measurement_index[index] = (measurement_index[index] + 1) % 5;

    ESP_LOGI(TAG, "Updated current measurement buffer for Sensor %d. Raw Current: %.3f A", sensor + 1, current);
}

/**
 * @brief Cyclically process current updates for all sensors.
 *
 * This function is called periodically to ensure the buffers for all sensors are updated.
 */
void current_sensing_process() {
    current_sensing_update(SENSOR_1);
    current_sensing_update(SENSOR_2);
}

/**
 * @brief Set the app-defined current limit.
 *
 * @param limit The current limit in amperes.
 */
void current_sensing_set_limit(float limit) {
    app_current_limit = (limit <= CURRENT_LIMIT_SAFE) ? limit : CURRENT_LIMIT_SAFE;
    ESP_LOGI(TAG, "App-defined current limit set to: %.2f A", app_current_limit);
}

/**
 * @brief Get the app-defined current limit.
 *
 * @return The app-defined current limit in amperes.
 */
float current_sensing_get_limit() {
    return app_current_limit;
}

/**
 * @brief Deinitialize the current sensing module.
 *
 * Cleans up resources used by the current sensing module.
 */
void current_sensing_deinit() {
    ESP_LOGI(TAG, "Deinitializing current sensing module...");
    free(adc_chars);
    ESP_LOGI(TAG, "Current sensing module deinitialized.");
}
