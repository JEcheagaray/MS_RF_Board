/**
 * @file voltage_sensing.c
 * @brief Voltage Sensing Software Component (SWC) for MS_RF_Board.
 *
 * This module monitors the voltage across the load and provides interfaces
 * for both raw and debounced voltage measurements.
 *
 * The debouncing logic is implemented internally and averages recent measurements
 * to provide a stable output.
 *
 * Note: The sensor pin VS_VOUT_2 (GPIO25) was not used because it is connected to ADC2,
 * which may cause conflicts when Wi-Fi is active. Only VS_VOUT_1 (GPIO33) is used
 * for consistent and reliable voltage sensing.
 *
 * @date 2025-01-26
 */

#include "voltage_sensing.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc_cal.h"

#define TAG "VoltageSensing"

// ADC Configuration
#define ADC_MAX_VALUE 4095
#define DEFAULT_VREF 1100  // Default reference voltage in mV
#define VOLTAGE_DIVIDER_RATIO 10.0  // Divider ratio (based on resistor configuration)

static esp_adc_cal_characteristics_t *adc_chars;  // ADC calibration characteristics

static float last_measurements[5];  // Buffer for debounced measurements
static int measurement_index = 0;

/**
 * @brief Initialize the voltage sensing module.
 *
 * Configures the ADC to read voltage measurements and sets up calibration.
 */
void voltage_sensing_init() {
    ESP_LOGI(TAG, "Initializing voltage sensing module...");

    adc1_config_width(ADC_WIDTH_BIT_12);  // 12-bit ADC resolution
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);  // GPIO33 (VS_VOUT_1)

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    ESP_LOGI(TAG, "Voltage sensing module initialized successfully.");
}

/**
 * @brief Read the raw voltage from the load.
 *
 * Converts the ADC reading to the actual load voltage in volts.
 *
 * @return The raw voltage in volts.
 */
static float read_raw_voltage() {
    int adc_reading = adc1_get_raw(ADC1_CHANNEL_5);  // GPIO33
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);  // Calibrated voltage in mV
    return (float)voltage / 1000.0 * VOLTAGE_DIVIDER_RATIO;  // Convert to actual load voltage in volts
}

/**
 * @brief Get the debounced voltage measurement.
 *
 * Averages the last 5 voltage measurements to provide a stable reading.
 *
 * @return The debounced load voltage in volts.
 */
float voltage_sensing_get_debounced() {
    float sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += last_measurements[i];
    }
    return sum / 5.0;
}

/**
 * @brief Update the debounced voltage buffer.
 *
 * Reads the raw voltage and updates the buffer for debouncing.
 */
void voltage_sensing_update() {
    float voltage = read_raw_voltage();
    last_measurements[measurement_index] = voltage;
    measurement_index = (measurement_index + 1) % 5;
    ESP_LOGI(TAG, "Updated voltage measurement buffer. Raw Voltage: %.2f V", voltage);
}

/**
 * @brief Run the voltage sensing task.
 *
 * Updates the debounced buffer and logs the voltage.
 */
void voltage_sensing_run() {
    voltage_sensing_update();
    float debounced_voltage = voltage_sensing_get_debounced();
    ESP_LOGI(TAG, "Debounced Voltage: %.2f V", debounced_voltage);
}

/**
 * @brief Deinitialize the voltage sensing module.
 *
 * Cleans up resources used by the voltage sensing module.
 */
void voltage_sensing_deinit() {
    ESP_LOGI(TAG, "Deinitializing voltage sensing module...");
    free(adc_chars);
    ESP_LOGI(TAG, "Voltage sensing module deinitialized.");
}
