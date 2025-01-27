/**
 * @file battery_monitoring.c
 * @brief Battery Monitoring Software Component (SWC) for MS_RF_Board.
 *
 * This module monitors the battery voltage of the MS_RF_Board, which uses three 18650 batteries in series.
 * It calculates the state of charge (SOC) based on the voltage readings and logs the battery status.
 *
 * @date 2025-01-26
 */

#include "battery_monitoring.h"
#include "esp_log.h"
#include "driver/adc.h"

#define TAG "BatteryMonitoring"

// ADC Configuration
#define BATTERY_ADC_CHANNEL ADC2_CHANNEL_9  // GPIO26 (D26) connected to VBATT_M1
#define ADC_MAX_VALUE 4095
#define ADC_VREF 1100  // ADC reference voltage in mV

// Battery Configuration
#define BATTERY_FULL_VOLTAGE 12.6  // 4.2V * 3 (full charge)
#define BATTERY_EMPTY_VOLTAGE 9.0  // 3.0V * 3 (minimum safe voltage)
#define VOLTAGE_DIVIDER_RATIO 2.0  // Voltage divider on the board

/**
 * @brief Initialize the battery monitoring module.
 *
 * Configures the ADC to read the battery voltage.
 */
void battery_monitoring_init() {
    ESP_LOGI(TAG, "Initializing battery monitoring module...");

    // Configure ADC2 for GPIO26
    adc2_config_channel_atten(BATTERY_ADC_CHANNEL, ADC_ATTEN_DB_11);  // Max input voltage ~3.6V

    ESP_LOGI(TAG, "Battery monitoring module initialized successfully.");
}

/**
 * @brief Read the battery voltage.
 *
 * Converts the ADC reading to the actual battery voltage using the configured voltage divider.
 *
 * @return The battery voltage in volts.
 */
static float read_battery_voltage() {
    int adc_reading;
    esp_err_t ret = adc2_get_raw(BATTERY_ADC_CHANNEL, ADC_WIDTH_BIT_12, &adc_reading);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read ADC: %s", esp_err_to_name(ret));
        return 0.0;
    }

    float voltage = ((float)adc_reading / ADC_MAX_VALUE) * ADC_VREF / 1000.0;  // Voltage at ADC pin
    voltage *= VOLTAGE_DIVIDER_RATIO;  // Compensate for voltage divider
    return voltage * 3.0;  // Scale up for the series configuration of three batteries
}

/**
 * @brief Calculate the state of charge (SOC).
 *
 * Uses the battery voltage to estimate the remaining charge percentage.
 *
 * @param voltage The battery voltage in volts.
 * @return The state of charge as a percentage (0-100%).
 */
static int calculate_soc(float voltage) {
    if (voltage >= BATTERY_FULL_VOLTAGE) {
        return 100;
    } else if (voltage <= BATTERY_EMPTY_VOLTAGE) {
        return 0;
    } else {
        return (int)(((voltage - BATTERY_EMPTY_VOLTAGE) / (BATTERY_FULL_VOLTAGE - BATTERY_EMPTY_VOLTAGE)) * 100);
    }
}

/**
 * @brief Run the battery monitoring task.
 *
 * Reads the battery voltage, calculates the SOC, and logs the battery status.
 */
void battery_monitoring_run() {
    float voltage = read_battery_voltage();
    if (voltage == 0.0) {
        ESP_LOGE(TAG, "Failed to read battery voltage.");
        return;
    }

    int soc = calculate_soc(voltage);

    ESP_LOGI(TAG, "Battery Voltage: %.2fV", voltage);
    ESP_LOGI(TAG, "State of Charge: %d%%", soc);

    if (voltage <= BATTERY_EMPTY_VOLTAGE) {
        ESP_LOGW(TAG, "Battery voltage critical! Please recharge.");
    }
}

/**
 * @brief Deinitialize the battery monitoring module.
 *
 * Cleans up resources used by the battery monitoring module.
 */
void battery_monitoring_deinit() {
    ESP_LOGI(TAG, "Deinitializing battery monitoring module...");
    // Additional cleanup if necessary
    ESP_LOGI(TAG, "Battery monitoring module deinitialized.");
}
