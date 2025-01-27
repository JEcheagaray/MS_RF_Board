/**
 * @file error_handling.h
 * @brief Header file for Error Handling Software Component (SWC) for MS_RF_Board.
 *
 * This header provides declarations for system error logging and recovery mechanisms
 * to ensure stability and prevent crashes.
 *
 * @date 2025-01-26
 */

#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

/**
 * @brief Error code for critical errors that require immediate recovery.
 */
#define ERROR_CODE_CRITICAL -1

/**
 * @brief Initialize the error handling module.
 *
 * Sets up error logging and recovery mechanisms.
 */
void error_handling_init(void);

/**
 * @brief Log an error message.
 *
 * Logs the provided error message with the ESP logging system.
 *
 * @param error_message The error message to log.
 */
void error_handling_log(const char *error_message);

/**
 * @brief Perform recovery actions for critical errors.
 *
 * Attempts to recover the system or gracefully shut down in the event of a critical error.
 *
 * @param error_code The error code associated with the critical error.
 */
void error_handling_recover(int error_code);

/**
 * @brief Deinitialize the error handling module.
 *
 * Cleans up resources used by the error handling module.
 */
void error_handling_deinit(void);

#endif // ERROR_HANDLING_H
