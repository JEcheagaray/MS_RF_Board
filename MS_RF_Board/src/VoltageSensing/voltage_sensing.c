#include "voltage_sensing.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define TAG "VoltageSensing"

// ADC Configuration
#define DEFAULT_VREF 1100  // Default reference voltage in mV
#define VOLTAGE_DIVIDER_RATIO 10.0  // Divider ratio (based on resistor configuration)

// ADC handle and calibration
static adc_oneshot_unit_handle_t adc_handle;
static adc_cali_handle_t cali_handle;

static float last_measurements[5];  // Buffer for debounced measurements
static int measurement_index = 0;

/**
 * @brief Initialize the voltage sensing module.
 *
 * Configures the ADC to read voltage measurements and sets up calibration.
 */
void voltage_sensing_init() {
    ESP_LOGI(TAG, "Initializing voltage sensing module...");

    // ADC oneshot configuration
    adc_oneshot_unit_init_cfg_t adc_config = {
        .unit_id = ADC_UNIT_1
    };
    adc_oneshot_new_unit(&adc_config, &adc_handle);

    // Configure channel
    adc_oneshot_chan_cfg_t channel_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12  // Updated to recommended value
    };
    adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_5, &channel_config);  // GPIO33 (VS_VOUT_1)

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
    int adc_raw = 0;
    int voltage = 0;  // Use `int` instead of `uint32_t` as required by `adc_cali_raw_to_voltage`

    adc_oneshot_read(adc_handle, ADC_CHANNEL_5, &adc_raw);

    if (cali_handle) {
        adc_cali_raw_to_voltage(cali_handle, adc_raw, &voltage);  // Calibrated voltage in mV
    } else {
        ESP_LOGW(TAG, "ADC calibration not available. Using raw value.");
        voltage = adc_raw;  // Use raw ADC reading if calibration is unavailable
    }

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
    if (cali_handle) {
        adc_cali_delete_scheme_line_fitting(cali_handle);
    }
    adc_oneshot_del_unit(adc_handle);
    ESP_LOGI(TAG, "Voltage sensing module deinitialized.");
}
