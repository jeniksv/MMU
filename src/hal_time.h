#pragma once

namespace HAL {

// TODO variable should be private in app context
/**
 * @var current_millis
 * @brief A global variable that holds the current time in milliseconds.
 *
 * This variable is intended to be updated by the system to reflect the current time.
 */
extern unsigned long current_millis;

/**
 * @brief Retrieves the current time in milliseconds.
 *
 * This function returns the value of the `current_millis` variable, which
 * represents the current time in milliseconds.
 *
 * @return The current time in milliseconds.
 */
unsigned long GetTime();

} // namespace HAL
