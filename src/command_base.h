/// @file command_base.h
#pragma once

#include "error_codes.h"
#include "progress_codes.h"
#include <stdint.h>

namespace logic {

/**
 * @class command_base
 * @brief Abstract base class for commands in a 3D printer control system.
 *
 * This class defines the interface and common functionality for commands used in the 3D printer control system.
 */
class command_base {
public:
    /**
     * @brief Constructor for the command_base class.
     *
     * Initializes the command base with default values.
     */
    command_base();

    /**
     * @brief Virtual destructor for the command_base class.
     */
    virtual ~command_base() = default;

    /**
     * @brief Resets the command with the given parameter.
     *
     * Calls the reset_helper method which must be implemented by derived classes.
     *
     * @param param A parameter to specify reset conditions.
     * @return True if the reset is successful, false otherwise.
     */
    bool reset(uint8_t param);

    /**
     * @brief Pure virtual method to reset the command.
     *
     * This method must be implemented by derived classes to reset the command.
     *
     * @param param A parameter to specify reset conditions.
     * @return True if the reset is successful, false otherwise.
     */
    virtual bool reset_helper(uint8_t param) = 0;

    /**
     * @brief Pure virtual method to execute a step of the command.
     *
     * This method must be implemented by derived classes to perform a step in the command.
     *
     * @return True if the command is finished, false otherwise.
     */
    virtual bool step() = 0;

    /**
     * @brief Gets the current state of the command.
     *
     * @return The current progress code representing the state of the command.
     */
    progress_code get_state() const;

    /**
     * @brief Gets the current error of the command.
     *
     * @return The current error code representing any error in the command.
     */
    error_code get_error() const;

protected:
    /**
     * @brief Checks if the tool index is within the specified range.
     *
     * This method checks if the provided tool index is within the valid range (0 to config::TOOL_COUNT).
     * If not, it sets the error to error_code::INVALID_TOOL.
     *
     * @param index The tool index to check.
     * @return True if the tool index is valid, false otherwise.
     */
    bool check_tool_index(uint8_t index);

    /**
     * @brief Processes the end of a command that finished successfully.
     *
     * This method is called to mark the command as finished successfully.
     */
    void finished_ok();

    progress_code _state;
    error_code _error;
    unsigned long _start_time;
};

} // namespace logic
