/**
 * @file nvm.h
 * @brief Header file for Non-Volatile Memory (NVM) Software Component (SWC) for MS_RF_Board.
 *
 * This header provides declarations for storing and retrieving persistent data
 * such as user configurations, frequency programs, and settings.
 *
 * @date 2025-01-26
 */

#ifndef NVM_H
#define NVM_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Initialize the NVM module.
 *
 * Prepares the non-volatile storage for reading and writing data.
 */
void nvm_init(void);

/**
 * @brief Save a key-value pair to NVM.
 *
 * @param key The key under which the value is stored.
 * @param value The string value to store.
 * @return true if the operation is successful, false otherwise.
 */
bool nvm_save(const char *key, const char *value);

/**
 * @brief Load a value from NVM.
 *
 * @param key The key associated with the value.
 * @param value Buffer to store the retrieved value.
 * @param length Length of the buffer.
 * @return true if the operation is successful, false otherwise.
 */
bool nvm_load(const char *key, char *value, size_t length);

/**
 * @brief Deinitialize the NVM module.
 *
 * Cleans up resources used by the NVM module.
 */
void nvm_deinit(void);

#endif // NVM_H
