/**
 * @file battery_monitoring.c
 * @brief Battery Monitoring Software Component (SWC) for MS_RF_Board.
 *
 * This module monitors the battery voltage of the MS_RF_Board, which uses three 18650 batteries in series.
 * It calculates the state of charge (SOC) based on the debounced voltage readings and logs the battery status.
 *
 * @date 2025-01-26
 */

#include "battery_monitoring.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define TAG "BatteryMonitoring"

// ADC Configuration
#define ADC_VREF 1100  // ADC reference voltage in mV
#define VOLTAGE_DIVIDER_RATIO 2.0  // Voltage divider on the board

// Battery Configuration
#define BATTERY_FULL_VOLTAGE 12.6  // 4.2V * 3 (full charge)
#define BATTERY_EMPTY_VOLTAGE 9.0  // 3.0V * 3 (minimum safe voltage)

// ADC Channel
#define BATTERY_ADC_CHANNEL ADC_CHANNEL_9  // GPIO26 (D26) connected to VBATT_M1

// Debouncing
#define DEBOUNCE_SAMPLES 5
static float last_measurements[DEBOUNCE_SAMPLES];
static int measurement_index = 0;

// ADC handles
static adc_oneshot_unit_handle_t adc_handle;
static adc_cali_handle_t cali_handle;

/**
 * @brief Initialize the battery monitoring module.
 *
 * Configures the ADC to read the battery voltage and sets up calibration.
 */
void battery_monitoring_init() {
    ESP_LOGI(TAG, "Initializing battery monitoring module...");

    // ADC oneshot configuration
    adc_oneshot_unit_init_cfg_t adc_config = {
        .unit_id = ADC_UNIT_1
    };
    adc_oneshot_new_unit(&adc_config, &adc_handle);

    // Configure channel
    adc_oneshot_chan_cfg_t channel_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12
    };
    adc_oneshot_config_channel(adc_handle, BATTERY_ADC_CHANNEL, &channel_config);

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

    // Initialize debounce buffer
    for (int i = 0; i < DEBOUNCE_SAMPLES; i++) {
        last_measurements[i] = 0.0;
    }

    ESP_LOGI(TAG, "Battery monitoring module initialized successfully.");
}

/**
 * @brief Read the raw battery voltage.
 *
 * Converts the ADC reading to the actual battery voltage in volts.
 *
 * @return The battery voltage in volts.
 */
static float read_raw_voltage() {
    int adc_raw = 0;
    int voltage = 0;

    adc_oneshot_read(adc_handle, BATTERY_ADC_CHANNEL, &adc_raw);

    if (cali_handle) {
        adc_cali_raw_to_voltage(cali_handle, adc_raw, &voltage);  // Calibrated voltage in mV
    } else {
        ESP_LOGW(TAG, "ADC calibration not available. Using raw value.");
        voltage = adc_raw;  // Use raw ADC reading if calibration is unavailable
    }

    return ((float)voltage / 1000.0) * VOLTAGE_DIVIDER_RATIO * 3.0;  // Scale to series battery voltage
}

/**
 * @brief Get the debounced battery voltage.
 *
 * Averages the last DEBOUNCE_SAMPLES voltage readings to provide a stable value.
 *
 * @return The debounced battery voltage in volts.
 */
static float get_debounced_voltage() {
    float sum = 0.0;
    for (int i = 0; i < DEBOUNCE_SAMPLES; i++) {
        sum += last_measurements[i];
    }
    return sum / DEBOUNCE_SAMPLES;
}

/**
 * @brief Update the debounce buffer with a new voltage measurement.
 */
static void update_debounce_buffer(float voltage) {
    last_measurements[measurement_index] = voltage;
    measurement_index = (measurement_index + 1) % DEBOUNCE_SAMPLES;
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
 * Reads the battery voltage, updates the debounce buffer, calculates the SOC, and logs the battery status.
 */
void battery_monitoring_run() {
    float raw_voltage = read_raw_voltage();
    update_debounce_buffer(raw_voltage);

    float debounced_voltage = get_debounced_voltage();
    int soc = calculate_soc(debounced_voltage);

    ESP_LOGI(TAG, "Battery Voltage (Raw): %.2fV", raw_voltage);
    ESP_LOGI(TAG, "Battery Voltage (Debounced): %.2fV", debounced_voltage);
    ESP_LOGI(TAG, "State of Charge: %d%%", soc);

    if (debounced_voltage <= BATTERY_EMPTY_VOLTAGE) {
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
    if (cali_handle) {
        adc_cali_delete_scheme_line_fitting(cali_handle);
    }
    adc_oneshot_del_unit(adc_handle);
    ESP_LOGI(TAG, "Battery monitoring module deinitialized.");
}
