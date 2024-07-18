#pragma once

#include "command_base.h"
#include <stdint.h>

namespace logic {

/**
 * @class no_command
 * @brief A command class that performs no action.
 *
 * This class represents a no-operation (no-op) command. It can be used as a placeholder
 * or default command that performs no actions when its methods are called.
 */
class no_command : public command_base {
public:
    /**
     * @brief Constructor for the no_command class.
     *
     * Initializes the no_command object.
     */
    no_command();

    /**
     * @brief Resets the command.
     *
     * This method is a no-op for this command.
     *
     * @param param This parameter is ignored.
     * @return Always returns true.
     */
    bool reset_helper(uint8_t param) override;

    /**
     * @brief Executes a step of the command.
     *
     * This method is a no-op for this command.
     *
     * @return Always returns true.
     */
    bool step() override;
};

} // namespace logic
