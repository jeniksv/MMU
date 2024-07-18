#pragma once

#include "command_base.h"
#include "config.h"
#include "feeding_system.h"
#include "registers.h"
#include "sensor.h"

namespace logic {

/**
 * @class concrete_command_base
 * @brief Abstract base class for concrete commands in a 3D printer.
 *
 * This class serves as a base for specific commands related to the feeding system
 * of a 3D printer. It provides common functionality and enforces the implementation
 * of reset and step methods in derived classes.
 */
class concrete_command_base : public command_base {
public:
    /**
     * @brief Constructor for the concrete_command_base class.
     *
     * Initializes the command with references to the feeding systems, hub filament sensor, and registers.
     *
     * @param _feeding_systems Array of feeding systems, one for each tool.
     * @param _hub_fsensor Reference to the hub filament sensor.
     * @param _registers Reference to the registers module.
     */
    concrete_command_base(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor& _hub_fsensor, modules::registers& _registers);

    /**
     * @brief Attempts to unstuck the filament.
     *
     * This method provides a generic mechanism to try to unstuck the filament in case of a jam.
     *
     * @return True if the filament was successfully unstuck, false otherwise.
     */
    bool unstuck();

    /**
     * @brief Resets the command with the given parameter.
     *
     * This is a pure virtual method that must be implemented by derived classes to reset the command.
     *
     * @param param A parameter to specify reset conditions.
     * @return True if the reset is successful, false otherwise.
     */
    virtual bool reset_helper(uint8_t param) = 0;

    /**
     * @brief Executes a step of the command.
     *
     * This is a pure virtual method that must be implemented by derived classes to perform a step in the command.
     *
     * @return True if the step is successful, false otherwise.
     */
    virtual bool step() = 0;

protected:
    modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT];
    modules::hub_filament_sensor& _hub_fsensor;
    modules::registers& _registers;
};

} // namespace logic
