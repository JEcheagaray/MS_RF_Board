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
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "CurrentSensing"

// ADC Configuration
#define DEFAULT_VREF 1100  // Default reference voltage in mV
#define CURRENT_SENSE_RESISTOR 0.015  // Shunt resistor value in ohms (15 mΩ)
#define CURRENT_SENSE_GAIN 20.0       // Amplification gain of the current sensing circuit

// Safety Thresholds
#define CURRENT_LIMIT_SAFE 0.1  // 100 mA human safety limit
static float app_current_limit = CURRENT_LIMIT_SAFE;  // Default to safety limit
static float last_measurements[2][5] = {{0}, {0}};  // Buffers for debouncing (Sensor1 and Sensor2)
static int measurement_index[2] = {0, 0};

// ADC handle and calibration
static adc_oneshot_unit_handle_t adc_handle;
static adc_cali_handle_t cali_handle;

// ADC Channels
#define SENSOR_1_CHANNEL ADC_CHANNEL_4  // GPIO32
#define SENSOR_2_CHANNEL ADC_CHANNEL_7  // GPIO35

/**
 * @brief Initialize the current sensing module.
 *
 * Configures the ADC to read current measurements and sets up calibration.
 */
void current_sensing_init() {
    ESP_LOGI(TAG, "Initializing current sensing module...");

    // ADC oneshot configuration
    adc_oneshot_unit_init_cfg_t adc_config = {
        .unit_id = ADC_UNIT_1
    };
    adc_oneshot_new_unit(&adc_config, &adc_handle);

    // Configure channels
    adc_oneshot_chan_cfg_t channel_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12  // Updated to recommended value
    };
    adc_oneshot_config_channel(adc_handle, SENSOR_1_CHANNEL, &channel_config);
    adc_oneshot_config_channel(adc_handle, SENSOR_2_CHANNEL, &channel_config);

    // ADC calibration configuration
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    if (adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize ADC calibration!");
        cali_handle = NULL;
    }

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
    int adc_raw = 0;
    int voltage = 0;  // Use `int` instead of `uint32_t` as required by `adc_cali_raw_to_voltage`

    if (sensor == SENSOR_1) {
        adc_oneshot_read(adc_handle, SENSOR_1_CHANNEL, &adc_raw);
    } else if (sensor == SENSOR_2) {
        adc_oneshot_read(adc_handle, SENSOR_2_CHANNEL, &adc_raw);
    }

    if (cali_handle) {
        adc_cali_raw_to_voltage(cali_handle, adc_raw, &voltage);  // Calibrated voltage in mV
    } else {
        ESP_LOGW(TAG, "ADC calibration not available. Using raw value.");
        voltage = adc_raw;  // Use raw ADC reading if calibration is unavailable
    }

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
    if (cali_handle) {
        adc_cali_delete_scheme_line_fitting(cali_handle);
    }
    adc_oneshot_del_unit(adc_handle);
    ESP_LOGI(TAG, "Current sensing module deinitialized.");
}
