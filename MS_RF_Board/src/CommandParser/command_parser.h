/**
 * @file command_parser.h
 * @brief Header file for Command Parser Software Component (SWC) for MS_RF_Board.
 *
 * This header provides the declarations for parsing and validating commands
 * received from the Bluetooth interface.
 *
 * @date 2025-01-26
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdbool.h>

/**
 * @brief Initialize the command parser module.
 *
 * Prepares the command parser for processing incoming commands.
 */
void command_parser_init(void);

/**
 * @brief Parse and validate a received command.
 *
 * @param command The command string received from Bluetooth.
 * @return true if the command is valid, false otherwise.
 */
bool command_parser_parse(const char *command);

/**
 * @brief Deinitialize the command parser module.
 *
 * Cleans up resources used by the command parser.
 */
void command_parser_deinit(void);

#endif // COMMAND_PARSER_H
