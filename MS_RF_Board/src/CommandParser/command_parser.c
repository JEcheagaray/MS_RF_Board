/**
 * @file command_parser.c
 * @brief Command Parser Software Component (SWC) for MS_RF_Board.
 *
 * This module parses and validates commands received from the Bluetooth interface.
 * It ensures commands are well-formed and forwards valid commands to the appropriate
 * system components.
 *
 * @date 2025-01-26
 */

#include "command_parser.h"
#include "esp_log.h"
#include <string.h>
#include <stdbool.h>

#define TAG "CommandParser"

/**
 * @brief Initialize the command parser module.
 *
 * Prepares the command parser for processing incoming commands.
 */
void command_parser_init() {
    ESP_LOGI(TAG, "Initializing command parser...");
    // Add any required initialization here
    ESP_LOGI(TAG, "Command parser initialized successfully.");
}

/**
 * @brief Parse and validate a received command.
 *
 * @param command The command string received from Bluetooth.
 * @return true if the command is valid, false otherwise.
 */
bool command_parser_parse(const char *command) {
    if (command == NULL || strlen(command) == 0) {
        ESP_LOGW(TAG, "Received an empty or null command.");
        return false;
    }

    ESP_LOGI(TAG, "Parsing command: %s", command);

    // Example: Validate command format (e.g., JSON, specific patterns)
    if (strncmp(command, "SET_FREQ", 8) == 0) {
        ESP_LOGI(TAG, "Command validated: SET_FREQ");
        return true;
    } else if (strncmp(command, "GET_STATUS", 10) == 0) {
        ESP_LOGI(TAG, "Command validated: GET_STATUS");
        return true;
    }

    ESP_LOGW(TAG, "Unknown command: %s", command);
    return false;
}

/**
 * @brief Deinitialize the command parser module.
 *
 * Cleans up resources used by the command parser.
 */
void command_parser_deinit() {
    ESP_LOGI(TAG, "Deinitializing command parser...");
    // Add any required cleanup here
    ESP_LOGI(TAG, "Command parser deinitialized.");
}
